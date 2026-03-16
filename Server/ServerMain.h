#pragma once
#include "Common.h"
#include "DBLogin.h"
#include <vector>
#include <thread>
#include <atomic>

class ServerMain {
public:
	ServerMain();
	~ServerMain();
	bool InitServer(int port);
	void AcceptClient();

private:
	SOCKET listen_sock;
	DBLogin db;
};