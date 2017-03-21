#include <string>
#include <iostream>

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#ifdef _WIN32
#include <Windows.h>	// Sleep
#include <mmsystem.h>	// timeBeginPeriod at winmm.lib
#pragma comment(lib, "winmm.lib") 
#endif

struct NetPeer
{
	bool wsaDataInited = false;
	WSADATA wsaData;
	SOCKET listener = INVALID_SOCKET;
	sockaddr_in myAddr;

	SOCKET client = INVALID_SOCKET;
	sockaddr_in remoteAddr;

	std::string recvData;
	std::string sendData;

	NetPeer(uint16_t port, int backlog = 10)
	{
		WORD sockVersion = MAKEWORD(2, 2);
		if (WSAStartup(sockVersion, &wsaData))
		{
			throw new std::exception("bad socket version.");
		}
		wsaDataInited = true;


		listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (listener == INVALID_SOCKET)
		{
			throw new std::exception("socket error.");
		}


		std::memset(&myAddr, 0, sizeof(myAddr));
		myAddr.sin_family = AF_INET;
		myAddr.sin_port = htons(port);
		myAddr.sin_addr.S_un.S_addr = INADDR_ANY;
		if (bind(listener, (LPSOCKADDR)&myAddr, sizeof(myAddr)) == SOCKET_ERROR)
		{
			throw new std::exception("bind error.");
		}


		if (listen(listener, backlog) == SOCKET_ERROR)
		{
			throw new std::exception("listen error.");
		}


		int nAddrlen = sizeof(remoteAddr);
		client = accept(listener, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (client == INVALID_SOCKET)
		{
			throw new std::exception("accept error.");
		}
		std::cout << "accept ip = " << inet_ntoa(remoteAddr.sin_addr) << std::endl;


		while (true)
		{
			int dataLen = 0;
			int left = sizeof(dataLen);

		LabBeginRecvHeader:
			int r = recv(client, (char*)&dataLen + sizeof(dataLen) - left, left, 0);
			if (r <= 0) return;
			left -= r;
			if (left) goto LabBeginRecvHeader;

			left = dataLen;
			recvData.resize(left);

		LabBeginRecvData:
			r = recv(client, recvData.data() + dataLen - left, left, 0);
			if (r <= 0) return;
			left -= r;
			if (left) goto LabBeginRecvData;

			// todo: FillMsg
			// todo: Call Update
			// todo: fill sendData

			dataLen = (int)sendData.size();
			left = sizeof(dataLen);

		LabBeginSendHeader:
			r = send(client, (char*)&dataLen + sizeof(dataLen) - left, left, 0);
			if (r <= 0) return;
			left -= r;
			if (left) goto LabBeginSendHeader;

			left = dataLen;

		LabBeginSendData:
			r = send(client, sendData.data() + dataLen - left, left, 0);
			if (r <= 0) return;
			left -= r;
			if (left) goto LabBeginSendData;

		}
	}

	~NetPeer()
	{
		if (client != INVALID_SOCKET)
		{
			closesocket(client);
		}
		if (listener != INVALID_SOCKET)
		{
			closesocket(listener);
		}
		if (wsaDataInited)
		{
			WSACleanup();
		}
	}
};
