# EasyQueue
A very lightweight thread-safe message queue implemented with C++11.

Header-only designed, easy to use.

Three components supplied:

+ class wait_signal : an improved condition variable

+ class message_queue : a high-level abstract & thread-safe queue

+ class single_thread_pool : a single thread pool for FIFO tasks