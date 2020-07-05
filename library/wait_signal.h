#ifndef EASY_QUEUE_WAIT_SIGNAL_H
#define EASY_QUEUE_WAIT_SIGNAL_H

#include <mutex>
#include <condition_variable>

namespace easy_queue
{

/// an improved condition variable based on std::condition_variable
/// I think that there MUST be a loop around std::condition_variable::wait for 2 reasions :
/// 1. for suprious wakeup;
/// 2. make sure that if std::condition_variable::notify is called before std::condition_variable::wait, wait could be quit by the predicate.
/// so this class will always use a loop to surround wait.
class wait_signal
{
public:
	void wait()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_cv.wait(lock, [&] { return _signaled; });
	}

	template<class Duration>
	bool wait_for(const Duration& rel_time)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		return _cv.wait_for(lock, rel_time, [&] { return _signaled; });
	}

	template<class Duration>
	bool wait_until(const Duration& timeout_time)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		return _cv.wait_until(lock, timeout_time, [&] { return _signaled; });
	}

	void signal(bool only_one)
	{
		{
			std::lock_guard<std::mutex> _(_mutex);
			_signaled = true;
		}

		if (only_one)
		{
			_cv.notify_one();
		}
		else
		{
			_cv.notify_all();
		}
	}

private:
	bool _signaled = false;
	std::condition_variable _cv;
	std::mutex _mutex;
};

}

#endif