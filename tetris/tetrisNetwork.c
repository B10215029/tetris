#include "tetrisNetwork.h"
#include "tetrisCLIRender.h"
#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN sizeof(GameData)
#define DEFAULT_PORT 27015

SOCKET connectSocket = INVALID_SOCKET;
struct sockaddr_in connect_addr;
int addr_len;
char buf[DEFAULT_BUFLEN];
GameData *myData, remoteData;

DWORD WINAPI RecvData(void* data) {
	int recv_len;

	while (1) {
		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', DEFAULT_BUFLEN);

		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(connectSocket, buf, DEFAULT_BUFLEN, 0, (struct sockaddr *) &connect_addr, &addr_len)) == SOCKET_ERROR) {
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		if (strcmp("bomb", buf) == 0) {
			myData->bomb++;
		}
		else {
			memcpy(&remoteData, buf, sizeof(GameData));
			dataUpdate = 1;
		}
	}
}

GameData* CreateServer(GameData* data) {
	struct sockaddr_in server;
	int recv_len;
	WSADATA wsa;

	addr_len = sizeof(connect_addr);
	myData = data;

	//Initialise winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//Create a socket
	if ((connectSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
	}

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(DEFAULT_PORT);

	//Bind
	if (bind(connectSocket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Wait client connect");

	//try to receive some data, this is a blocking call
	if ((recv_len = recvfrom(connectSocket, buf, DEFAULT_BUFLEN, 0, (struct sockaddr *) &connect_addr, &addr_len)) == SOCKET_ERROR) {
		printf("recvfrom() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	HANDLE thread = CreateThread(NULL, 0, RecvData, NULL, 0, NULL);
	return &remoteData;
}

GameData* ConnectServer(GameData* data, char* serverIP) {
	char message[DEFAULT_BUFLEN];
	WSADATA wsa;

	addr_len = sizeof(connect_addr);
	myData = data;

	//Initialise winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//create socket
	if ((connectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//setup address structure
	memset((char *)&connect_addr, 0, sizeof(connect_addr));
	connect_addr.sin_family = AF_INET;
	connect_addr.sin_port = htons(DEFAULT_PORT);
	connect_addr.sin_addr.S_un.S_addr = inet_addr(serverIP);

	if (sendto(connectSocket, "aaa", strlen("aaa"), 0, (struct sockaddr *) &connect_addr, addr_len) == SOCKET_ERROR) {
		printf("sendto() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	HANDLE thread = CreateThread(NULL, 0, RecvData, NULL, 0, NULL);
	return &remoteData;
}

void Disconnect() {
	closesocket(connectSocket);
	WSACleanup();
}

void SendData(GameData* data) {
	if (connectSocket == INVALID_SOCKET)
		return;
	if (sendto(connectSocket, data, sizeof(GameData), 0, (struct sockaddr *) &connect_addr, addr_len) == SOCKET_ERROR) {
		printf("sendto() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}

void SendBomb() {
	if (connectSocket == INVALID_SOCKET)
		return;
	if (sendto(connectSocket, "bomb", 5, 0, (struct sockaddr *) &connect_addr, addr_len) == SOCKET_ERROR) {
		printf("sendto() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}
