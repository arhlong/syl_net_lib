#include "SocketServer.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>

//declare the local functions
static int setnonblocking(int fd);

SocketServer::SocketServer(int port)
	:port_(port),backlog_(5),sockfd_(-1),epfd_(-1),brun(false)
{
}

SocketServer::~SocketServer()
{
	if(sockfd_ > 0) close(sockfd_);
	if(epfd_ > 0) close(epfd_);
}

void SocketServer::setBacklog(int val)
{
	backlog_ = val;
}

void SocketServer::setReadcb(Readcb func)
{
	readcb_ = func;
}

void SocketServer::setWritecb(Writecb func)
{
	writecb_ = func;
}

int SocketServer::listen_()
{
	if(sockfd_ < 0) return -1;
	return listen(sockfd_,backlog_);
}

int SocketServer::bind_()
{
	sockfd_ = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd_ < 0) return -1;
	struct sockaddr_in sin;
	bzero(&sin,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port_);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);

	return bind(sockfd_,(struct sockaddr*)&sin,sizeof(sin));
}

void SocketServer::stop()
{
	if(brun) brun = false;
}

void SocketServer::run()
{
	if(bind_() < 0) throw "bind fail!";
	if(listen_() < 0) throw "listen fail!";

	const int EPOLL_MAX_SIZE = 1024;
	epfd_ = epoll_create1(0);
	if(epfd_ < 0) throw "epoll_create1 error!";
	struct epoll_event evs[EPOLL_MAX_SIZE];

	brun = true;
	int nfds;
	while(brun)
	{
		nfds = epoll_wait(epfd_,evs,EPOLL_MAX_SIZE,-1);
		if(nfds < 0 && errno == EINTR)
		{
			perror("epoll_wait");
			continue;
		}
		else if(nfds < 0)
		{
			throw "epoll_wait error!";
		}
		else
		{
			for(int i = 0; i < nfds; i ++)
			{
				if(sockfd_ == evs[i].data.fd)
				{
					struct sockaddr_in clientaddr;
					socklen_t len = sizeof(clientaddr);
					int clientfd = accept(sockfd_,(struct sockaddr*)&clientaddr,&len);
					if(clientfd < 0)
					{
						perror("accept");
						continue;
					}

					if(addfd_(clientfd,EPOLLIN,0) < 0) throw "add fd to event queue error!";
				}
				else if(evs[i].events & EPOLLIN)
				{
					readcb_(evs[i].data.fd);
				}
				else if(evs[i].events & EPOLLOUT)
				{
					writecb_(evs[i].data.fd);
				}
			}
		}
	}
}

static int setnonblocking(int fd)
{
	int oldop = fcntl(fd,F_GETFL);
	int newop = oldop | O_NONBLOCK;
	return fcntl(fd,F_SETFL,newop);
}

int SocketServer::addfd_(int fd,int op,bool et)
{
	setnonblocking(fd);
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = op;
	if(et)
	{
		ev.events |= EPOLLET;
	}
	return epoll_ctl(epfd_,EPOLL_CTL_ADD,fd,&ev);
}
