#ifndef _SOCKET_SERVER_H_
#define _SOCKET_SERVER_H_
#include <string>
#include <functional>

using std::string;
using std::function;

class SocketServer
{
	typedef function<int(int)> Readcb;  //int readcb(int fd);
	typedef function<int(int)> Writecb; //int writecb(int fd);
public:
	explicit SocketServer(int port);
	virtual ~SocketServer();
	SocketServer(const SocketServer& obj) = delete;
	SocketServer& operator=(const SocketServer& obj) = delete;

public:
	void setBacklog(int val);
	void setReadcb(Readcb func);
	void setWritecb(Writecb func);
	void run();
	void stop();

private:
	int listen_();
	int bind_();
    int addfd_(int fd,int op,bool et); //添加fd到epoll队列，et==true表示使用ET模式
	//int accept_();

private:
	Readcb readcb_;
	Writecb writecb_;

private:
	int    port_;
	int    backlog_;
	int    sockfd_;
	int    epfd_;
	bool   brun;
};

#endif
