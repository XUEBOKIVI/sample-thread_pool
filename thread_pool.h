#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <queue>

template <class T>
class thread_pool
{
public:
	thread_pool(int nums);
	~thread_pool();
	bool append_task(T* task);
	T* get_task();
private:
	int thread_nums;
	std::vector<std::thread> threads;
	std::queue<T *> task_queue;
	std::mutex queue_mutex; //互斥量
	std::condition_variable queue_cond; //条件变量
	bool is_stop; //是否结束线程

	void worker(); //线程内工作函数
};


template<class T>
thread_pool<T>::thread_pool(int nums)
	: is_stop(false), thread_nums(nums)
{
	for (int i = 0; i < thread_nums; i++)
	{
		threads.push_back(std::thread(&thread_pool::worker, this));
	}
}


template<class T>
thread_pool<T>::~thread_pool()
{
  is_stop = true;
	for (int i = 0; i < threads.size(); i++)
	{
		threads[i].join();
	}
}


template<class T>
bool thread_pool<T>::append_task(T* task)
{
	std::lock_guard<std::mutex> guard(queue_mutex);
	if (!task_queue.empty())
	{
		task_queue.push(task);
	}
	else
	{
		task_queue.push(task);
		queue_cond.notify_one();
	}

	return true;
}

template<class T>
void thread_pool<T>::worker()
{
	while (!is_stop)
	{
		T* task = get_task();
		task->do_it();
		delete task;
	}
}

template<class T>
T* thread_pool<T>::get_task()
{
	T* task;
	std::unique_lock <std::mutex> lk(queue_mutex);

	while (task_queue.empty())
	{
		queue_cond.wait(lk);
	}

	task = task_queue.front();
	task_queue.pop();

	return task;
}
#endif // !THREAD_POOL
