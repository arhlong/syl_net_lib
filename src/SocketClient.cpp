#include "SocketServer.h"
#include <iostream>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

SocketServer::SocketServer(int port)
    : port_(port),sockfd_(-1),backlog_(5)
{
}

SocketServer::~SocketServer()
{
}

void SocketServer::run()
{
}

void SocketServer::listen_()
{
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in sin;
	bzero(sin,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port_);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);

}
