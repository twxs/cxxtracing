#define CXXTRACING_ENABLED
#include <cxxtracing/cxxtracing.h>
#include <iostream>
#include <thread>
#include <string>

void f1(int n)
{
	CXXTRACING_THREAD_NAME("GUIThread");
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	CXXTRACING_END_ASYNC("ASYNC");
	{
		CXXTRACING_ACTIVITY("Import");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	

	{
		CXXTRACING_ACTIVITY("Engraving");
		{
			CXXTRACING_ACTIVITY("BuildingSceneGraph");
			std::this_thread::sleep_for(std::chrono::milliseconds(42));
		}
		{
			CXXTRACING_ACTIVITY("Collision");
			std::this_thread::sleep_for(std::chrono::milliseconds(32));
		}

	}

	for (int i = 0; i < 5; ++i) {
		CXXTRACING_ACTIVITY("Render");
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		{
			CXXTRACING_ACTIVITY("painting");
			std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
		}
	}
}

void f2(int& n)
{
	CXXTRACING_THREAD_NAME("AudioThread");
	{
		CXXTRACING_ACTIVITY("loading Samples");
		for (int i = 0; i < 5; ++i) {
			CXXTRACING_MARK((std::string("sample#") + std::to_string(i)).c_str());
			std::this_thread::sleep_for(std::chrono::milliseconds(10 + 10*i));
		}
	}
	for (int i = 0; i < 10; ++i) {
		{
			CXXTRACING_ACTIVITY("FillBuffer");
			++n;
			
			std::this_thread::sleep_for(std::chrono::milliseconds(210));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	}
}

int main()
{
	CXXTRACING_INIT("c:/tmp/trace.json");
	CXXTRACING_PROCESS_NAME("sample1");
	CXXTRACING_THREAD_NAME("Main");
	CXXTRACING_BEGIN_ASYNC("ASYNC");
	int n = 0;
	{
		CXXTRACING_ACTIVITY("Main");
	
		std::thread t1(f1, n + 1); // pass by value
		std::thread t2(f2, std::ref(n)); // pass by reference

		t1.join();
		t2.join();
		std::cout << "Final value of n is " << n << '\n';
	}
	
	CXXTRACING_SHUTDOWN();
}
  