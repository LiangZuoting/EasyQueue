#ifndef EASY_QUEUE_MESSAGE_QUEUE_H
#define EASY_QUEUE_MESSAGE_QUEUE_H

#include <mutex>
#include <functional>

namespace easy_queue
{

/// a high-level abstract & thread-safe queue
/// you could use any type for underlying container,
/// what this class does is just making sure all operations on this container is thread-safe.
template <class Container>
class message_queue
{
public:
	void do_work(std::function<void(Container&)> func)
	{
		std::lock_guard<std::mutex> _(_mutex);
		func(_queue);
	}

private:
	Container _queue;
	std::mutex _mutex;
};

}

#endif