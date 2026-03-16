#include "ServerMain.h"

ServerMain::ServerMain() : listen_sock(INVALID_SOCKET) {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		err_quit("WSAStartup() 실패");
	}
}

ServerMain::~ServerMain() {

	//listen socket 정리
	if (listen_sock != INVALID_SOCKET) {
		closesocket(listen_sock);
		listen_sock = INVALID_SOCKET;
	}
	WSACleanup();
}

// -------------------------------
// 서버 초기화
// -------------------------------

bool ServerMain::InitServer(int port) {
	
	int retval;

	// 소켓 생성
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) {
		err_quit("socket() 실패");
		return false;
	}

	// bind
	sockaddr_in serveraddr{};
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(static_cast<u_short>(port));
	retval = bind(listen_sock, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		err_quit("bind()");
		return false;
	}

	// listen
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		err_quit("listen()");
		return false;
	}

	if (!db.ConnectDB())
	{
		std::cout << "DB 연결 실패" << "\n";
		return false;
	}

	std::cout << "DB 연결 성공" << "\n";

	// 디비 테스트
	//db.RegisterUser("player1", "1234");

	//if (db.LoginUser("player1", "1234"))
	//{
	//	std::cout << "로그인 성공\n";
	//}
	//else
	//{
	//	std::cout << "로그인 실패\n";
	//}


	return true;
}


// -------------------------------
// 클라이언트 접속 대기
// -------------------------------
void ServerMain::AcceptClient() {

	while (1) {

		sockaddr_in clientaddr{};
		int addrlen = sizeof(clientaddr);
		HANDLE hThread;

		// accept
		SOCKET client_sock = accept(listen_sock, reinterpret_cast<sockaddr*>(&clientaddr), &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			continue;
		}

		// 접속한 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN]{};
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		int cport = ntohs(clientaddr.sin_port);
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", addr, cport);

		// clients에 등록 
		// 클라이언트 별 스레드 생성
	}
}