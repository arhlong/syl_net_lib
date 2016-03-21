#ifndef _MSG_QUEUE_H_
#define _MSG_QUEUE_H_

#include <queue>
#include <mutex>
#include <condition_variable>

using std::mutex;
using std::condition_variable;
using std::unique_lock;
using std::queue;

template <typename T>
class MsgQueue
{
public:
	MsgQueue();
	~MsgQueue();
	MsgQueue(MsgQueue& mq) = delete;
	MsgQueue& operator=(const MsgQueue& q) = delete;

public:
	void push(T pmsg);
	T pop();

private:
	queue<T> q_;
	bool ready_;
	mutex mtx_;
	condition_variable cv_;
};

template <typename T>
MsgQueue<T>::MsgQueue()
{
	ready_ = false;
}

template <typename T>
MsgQueue<T>::~MsgQueue()
{
}

template <typename T>
void MsgQueue<T>::push(T pmsg)
{
	unique_lock<mutex> lck(mtx_);
	q_.push(pmsg);
	ready_ = true;
    cv_.notify_one();
}

template <typename T>
T MsgQueue<T>::pop()
{
	unique_lock<mutex> lck(mtx_);
	while(!ready_)
	{
		cv_.wait(lck);
	}

	T ret = q_.front();
	q_.pop();
	ready_ = false;
	return ret;
}

#endif
