#ifndef _SOCKET_SERVER_H_
#define _SOCKET_SERVER_H_
#include <string>

using std::string;

class SocketServer
{
public:
	explicit SocketServer(int port);
	~SocketServer();
	SocketServer(const SocketServer& obj) = delete;
	SocketServer& operator=(const SocketServer& obj) = delete;

publice:
	void run();

private:
	void listen_();
	void bind_();
	int  accept_();

private:
	string ip_;
	int    backlog_;
	int    sockfd_;
	int    port_;
};

#endif
