#include "MsgQueue.h"
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

using std::string;
using std::shared_ptr;
MsgQueue<shared_ptr<string>> g_mq;

void push_info()
{
	int count = 0;
	while(1)
	{
		//if(count > 10) break;
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		shared_ptr<string> pstr(new string(string("test") + std::to_string(count)));
		g_mq.push(pstr);
		std::cout<<"push:"<<*pstr<<std::endl;
		++count;
	}
}

void print_info()
{
	while(1)
	{
		shared_ptr<string> pstr = g_mq.pop();
		std::cout<<"thread id:"<<std::this_thread::get_id()<<",print:"<<*pstr<<std::endl;
	}
}

#if 0
int main()
{
	using std::thread;
	using std::vector;
	thread tmain(push_info);
	//thread tprint(print_info);
    vector<thread> tprints;
	for(int i = 0; i < 10; i ++)
	{
		tprints.push_back(thread(print_info));
	}

	tmain.join();
	for(auto &t : tprints)
	{
		t.join();
	}
	//tprint.join();
}
#endif
