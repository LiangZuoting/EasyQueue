#ifndef EASY_QUEUE_SINGLE_THREAD_POOL_H
#define EASY_QUEUE_SINGLE_THREAD_POOL_H

#include <list>
#include <functional>
#include <mutex>
#include <thread>
#include "wait_signal.h"
#include "message_queue.h"

namespace easy_queue
{

using runnable = std::function<void()>;

/// a single thread pool based on wait_signal and message_queue.
/// enqueue all tasks and then process them FIFO.
template<class Container = std::list<runnable>>
class single_thread_pool
{
public:
#define LOCK std::lock_guard<std::mutex> _(_mutex);

	void start()
	{
		{
			LOCK
			if (!_stoped)
			{
				return;
			}
			_stoped = false;
		}
		_thread = std::thread([&] { this->thread_func(); });
	}

	void push(runnable r)
	{
		{
			LOCK
			if (_stoped)
			{
				return;
			}
		}
		_queue.do_work([=](Container& queue)
			{
				queue.push_back(r);
			});
		_signal.signal(false);
	}

	void stop()
	{
		{
			LOCK
			if (_stoped)
			{
				return;
			}
			_stoped = true;
		}

		_queue.do_work([] (Container& queue)
			{
				queue.clear();
			});

		_signal.signal(false);

		if (_thread.joinable())
		{
			_thread.join();
		}
	}

private:
	void thread_func()
	{
#define BREAK_IF_STOP \
{ \
	LOCK \
	if (_stoped) \
	{ \
		break; \
	} \
}

		while (true)
		{
			BREAK_IF_STOP

			_signal.wait();

			Container temp;
			_queue.do_work([&] (Container& queue)
				{
					queue.swap(temp);
				});
			if (temp.empty())
			{
				continue;
			}
			for (auto& f : temp)
			{
				BREAK_IF_STOP
				f();
			}
		}
	}

	wait_signal _signal;
	message_queue<Container> _queue;
	std::thread _thread;
	std::mutex _mutex;
	bool _stoped = true;
};

}

#endif