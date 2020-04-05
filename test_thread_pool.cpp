#include "thread_pool.h"

#include <iostream>
#include <mutex>
#include <thread>

std::mutex cout_mutex;

class task
{
public:
	task(int n) : num(n) {}
	~task() {}
	void do_it()
	{
		std::lock_guard<std::mutex> guard(cout_mutex);
		std::cout << "我是第" << num << "个任务！！" << "我的线程ID: " << std::this_thread::get_id() << std::endl;
	}

private:
	int num;
};


int main()
{
	int thread_nums = 10; //线程池的线程数
	thread_pool<task> my_thread_pool(thread_nums);
	for (int i = 0; i < 1000; i++)
	{
		my_thread_pool.append_task(new task(i));
	}

	while(1)
	{ }
	return 0;
}
