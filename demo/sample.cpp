#include <iostream>
#include <list>
#include "../library/single_thread_pool.h"

using namespace std;
using namespace easy_queue;

void main()
{
	single_thread_pool<> stp;
	stp.start();

#define TIMES 20

	thread t1([&]
		{
			for (int i = 0; i < TIMES; ++i)
			{
				stp.push([]
					{
						cout << "t1 abcdefg" << endl;
					});
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}
		});

	thread t2([&]
		{
			for (int i = 0; i < TIMES; ++i)
			{
				stp.push([]
					{
						cout << "t2 hijklmn" << endl;
					});
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
			}
		});

	thread t3([&]
		{
			for (int i = 0; i < TIMES; ++i)
			{
				stp.push([]
					{
						cout << "t3 opqrstu" << endl;
					});
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
		});

	thread t4([&]
		{
			for (int i = 0; i < TIMES; ++i)
			{
				stp.push([]
					{
						cout << "t4 vwxyz" << endl;
					});
				std::this_thread::sleep_for(std::chrono::milliseconds(350));
			}
		});

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	stp.stop();

	cout << "all done" << endl;

	getchar();
}