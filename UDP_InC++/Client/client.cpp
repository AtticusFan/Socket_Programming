#include<winsock2.h>
#define _WIN32_WINNT 0x501
#include <ws2tcpip.h>
#include <iostream>
#include <windows.h>
#define BUFFERLENGTH (8192)

using namespace std;
SOCKET mySocket;
char *targetIP = (char*) malloc(sizeof(char) * 20);
struct addrinfo server, *result = NULL, *ptr = NULL;
char *targetPort = (char*) malloc(sizeof(char) * 10);
int bufferLength = BUFFERLENGTH;

void receiveData(){
	char receiveBuffer[BUFFERLENGTH+1];
	int receiveLength;
	int state = 0;
	FILE* fptr = 0;
	int receivedBytes = 0;
	char *filename = (char*) malloc(sizeof(char) * 128);
	do {
		receiveLength = recvfrom(mySocket, receiveBuffer, bufferLength, 0, result->ai_addr, (int*) &result->ai_addrlen);
		if(receiveLength > 0)
		{
			if(state == 0)
			{
				if(strncmp(receiveBuffer, "Filename: ", 10) == 0)
				{
					strncpy(filename, receiveBuffer + 10, receiveLength - 10 + 1);
					fptr = fopen(filename, "wb");
					state = 1;
				}
				else if (strcmp(receiveBuffer, "Error") == 0)
				{
					cout << "Server encountered error" << endl;
					break;
				}
				else
					cout << "Server response: " << receiveBuffer << endl;
			}
			else if(state == 1)
			{
				if(strncmp(receiveBuffer, "Over", 4) == 0)
				{
					cout << "Receive over message, connection is going to be closed" << endl;
					cout << "Received bytes of file: " << receivedBytes << endl;
					fclose(fptr);
					break;
				}
				else
				{
					fwrite(receiveBuffer, sizeof(char), receiveLength, fptr);
					cout << "Receive bytes: " << receiveLength << endl;
					receivedBytes += receiveLength;
				}
			}
		} 
		else
			cout << "Connect error" << endl;
	} 
	while(receiveLength > 0);
} 
int main() {
	char *hello = (char*) malloc(sizeof(char) * 10);
	strcpy(targetPort, "25836");
	strcpy(hello, "Hello!");
	cout << "Please enter server ip: ";
	cin >> targetIP;
	 
	//targetIP='127.0.0.1';
	cout << "Connection with port " << targetPort << endl;

	// winsock ªì©l
	WSADATA windowsSocketInit;
	cout << "Winsock initializing..." << endl;
	if (WSAStartup(MAKEWORD(2, 2), &windowsSocketInit) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		shutdown(mySocket, SD_BOTH);
		closesocket(mySocket);
    	WSACleanup();
	}
	cout << "Winsock initialized." << endl;
	ZeroMemory( &server, sizeof(server) );
	server.ai_family = AF_UNSPEC;
    server.ai_socktype = SOCK_DGRAM;
    server.ai_protocol = IPPROTO_UDP;
    int addrlen = sizeof(struct addrinfo);
	int getaddrResult = getaddrinfo(targetIP, targetPort, &server, &result);
	if ( getaddrResult != 0 )
	{
        printf("getaddrinfo failed with error: %d\n", getaddrResult);
        shutdown(mySocket, SD_BOTH);
		closesocket(mySocket);
    	WSACleanup();
    }
    cout << "getaddrinfo successed." << endl;
	for(ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		cout << "Socket initializing..." << endl;
		mySocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if(mySocket == INVALID_SOCKET) {
			printf("Could not create socket : %d" , WSAGetLastError());
			continue;
		}
		printf("Socket created.\n");
	}
	sendto(mySocket, hello, strlen(hello), 0, result->ai_addr, result->ai_addrlen);
	receiveData();
	shutdown(mySocket, SD_BOTH);
	closesocket(mySocket);
    WSACleanup();
	system("pause");
	return 0;
}
