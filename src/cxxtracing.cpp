
#include <cxxtracing/cxxtracing.h>
#include <string>
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <sstream>
#include <map>
#include <fstream>



static std::map <std::thread::id, std::string> _threadNames;
static std::string _processName = "Process";
static std::string _category = "Category";
static std::thread receiver;


static const std::string& thread_name() {
	auto id = std::this_thread::get_id();
	auto it = _threadNames.find(id);
	if (it != _threadNames.end()){
		return it->second;
	}
	auto name = std::to_string(id.hash());
	_threadNames.emplace_hint(it, std::make_pair(id, name));
	return _threadNames[id];
}


enum Phase {
	PhaseBegin,
	PhaseEnd,
	PhaseMark,
	PhaseBeginAsync,
	PhaseEndAsync
};

struct ProfileBlock {
	std::string category;
	std::string process;
	std::string thread;
	std::string name;
	int64_t timestamp;
	std::map<std::string, std::string> args;
	Phase phase;

	ProfileBlock(const std::string name, Phase phase) {
		auto start = std::chrono::system_clock::now();
		this->process = _processName;
		this->thread = thread_name();
		this->name = name;
		this->timestamp = start.time_since_epoch().count();
		this->phase = phase;
	}
	std::string phase_string() const {
		switch (phase)
		{
		case PhaseBegin:
			return "B";
			break;
		case PhaseEnd:
			return "E" ;
			break;
		case PhaseMark:
			return "I";
			break;
		case PhaseBeginAsync:
			return "S";
			break;
		case PhaseEndAsync:
			return "F";
			break;
		default:
			break;
		}
		return "I";

	}

	std::string to_string() const{
		std::stringstream str;
		str << "{ ";
		str << "\"cat\": \"" << "category" << "\", ";
		str << "\"pid\": \"" << process << "\", ";
		str << "\"tid\": \"" << thread << "\", ";
		str << "\"ts\": " << timestamp << ", ";
		str << "\"ph\": \"" << phase_string() << "\", ";
		str << "\"name\": \"" << name << "\", ";
		str << "\"args\": {" "} ";
		str << "} ";
			
		return str.str();
	}

};

static std::vector<ProfileBlock> _queue;

static std::mutex _mutex;

void send_block(const ProfileBlock&b) {
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push_back(b);	
}

std::shared_ptr<void> CXX_TRACING_API make_activity(const char* name) {		
	std::shared_ptr<void> raii = std::shared_ptr<void>(nullptr, [=](void*){ send_block(ProfileBlock(name, PhaseEnd)); });
	send_block(ProfileBlock(name, PhaseBegin));
	return raii;
}

void CXX_TRACING_API begin_activity(const char *name) {
	send_block(ProfileBlock(name, PhaseBegin));
}
void CXX_TRACING_API end_activity(const char *name) {
	send_block(ProfileBlock(name, PhaseEnd));

}
void CXX_TRACING_API add_mark(const char *name) {
	send_block(ProfileBlock(name, PhaseMark));
}
void CXX_TRACING_API set_process_name(const char *) {

}
void CXX_TRACING_API set_thread_name(const char * name) {
	_threadNames[std::this_thread::get_id()] = name;
}

void CXX_TRACING_API set_category_name(const char *) {

}

static bool running = true;
void receive() {
	std::fstream f;
	f.open("c:\\tmp\\trace.json", std::ios_base::out | std::ios_base::trunc);
	bool b = f.is_open();
	f << "{\"traceEvents\": [\n";
	std::string sep;
	while (running) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		decltype(_queue) copy;
		{
			std::lock_guard<std::mutex> lock(_mutex);
			std::swap(_queue, copy);
		}

		for (auto & block : copy) {
			
			f << sep << std::endl << block.to_string();
			if (sep.empty())
				sep = ",";
		}
		
	}
	f << "\n]}\n";

}

CXX_TRACING_EXPORT void CXX_TRACING_API init() {
	std::swap(receiver, std::thread(receive));
}
CXX_TRACING_EXPORT void CXX_TRACING_API shutdown() {
	running = false;
	receiver.join();
}