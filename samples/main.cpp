#include <cxxtracing/cxxtracing.h>
#include <iostream>
#include <thread>
#include <string>

void f1(int n)
{
	set_thread_name("thread1");
	for (int i = 0; i < 5; ++i) {
		auto _a = make_activity("executing");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		auto _b = make_activity("subexecuting");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void f2(int& n)
{
	set_thread_name("thread2");
	{
		auto _a = make_activity("loading");
		for (int i = 0; i < 5; ++i) {
			add_mark((std::string("sample#") + std::to_string(i)).c_str());
			std::this_thread::sleep_for(std::chrono::milliseconds(10 + 10*i));
		}
	}
	for (int i = 0; i < 50; ++i) {
		{
			auto _a = make_activity("executing");
			++n;
			
			std::this_thread::sleep_for(std::chrono::milliseconds(210));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	}
}

int main()
{
	init();
	set_process_name("sample1");

	int n = 0;
	
	std::thread t1(f1, n + 1); // pass by value
	std::thread t2(f2, std::ref(n)); // pass by reference
	
	t1.join();
	t2.join();
	std::cout << "Final value of n is " << n << '\n';
	shutdown(); 
}
