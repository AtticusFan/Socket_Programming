#include <iostream>
#include <winsock2.h>
#define BUFFERLENGTH (8192)

using namespace std;
SOCKET mySocket, receiveSocket;
char fileOpenBuffer[BUFFERLENGTH + 1] = {0};
char receiveBuffer[BUFFERLENGTH + 1] = {0};
unsigned short listeningPort = 25836;
struct sockaddr_in client;
int addrlen = sizeof(struct sockaddr), bufferLength = BUFFERLENGTH;


void clearBuffer(char *input)
{
	for(int i = 0;i < BUFFERLENGTH;i++)
		input[i] = '\0';
}
void sendFile() {
	FILE* fptr = fopen("Readme.txt", "rb");
	//FILE* fptr = fopen("switch_ad.mp4", "rb");
	
	if(fptr != 0) {
		int transformedBytes = 0;

		
		char *fileName = (char*) malloc(sizeof(char) * 64);
		sendto(mySocket, fileName, strlen(fileName), 0, (struct sockaddr*) &client, addrlen);

		cout << "Send start." << endl;
		//傳送資料
		while(feof(fptr) == 0)
		{
			clearBuffer(fileOpenBuffer);
			int readBytes = fread(fileOpenBuffer, sizeof(char), bufferLength, fptr);
			transformedBytes += readBytes;
			cout << "Send data length: " << readBytes << endl;
			sendto(mySocket, fileOpenBuffer, readBytes, 0, (struct sockaddr*) &client, addrlen);
		}
		fclose(fptr);
		cout << "Total send data bytes: " << transformedBytes << endl;
		cout << "Data transfer over" << endl;
		sendto(mySocket, "Over", strlen("Over"), 0, (struct sockaddr*) &client, addrlen);
	}
	else
	{
		cout << "Open file failed." << endl;
		sendto(mySocket, "Error", strlen("Error"), 0, (struct sockaddr*) &client, addrlen);
	}

}
void closeServer() {
	if(mySocket > 0) {
		shutdown(mySocket, SD_BOTH);
		closesocket(mySocket);
	}
	WSACleanup();
	exit(-1);
}
int main() {
	//初始化winsock 
	WSADATA windowsSocketInit;
	cout << "Winsock initializing..." << endl;
	if (WSAStartup(MAKEWORD(2, 2), &windowsSocketInit) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		closeServer();
	}
	cout << "Winsock initialized." << endl;
	//初始化socket
	cout << "Socket initializing..." << endl;
	mySocket = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP );
	if(mySocket == INVALID_SOCKET) 
	{ 
		printf("Could not create socket : %d" , WSAGetLastError());
		closeServer();
	}
	printf("Socket created.\n");
	//將IP從十進制轉二進制 
	client.sin_addr.s_addr = inet_addr("0.0.0.0"); 
	client.sin_family = AF_INET;
	client.sin_port = htons(listeningPort);

	int bindResult = bind(mySocket , (struct sockaddr*) &client, sizeof(struct sockaddr_in));
	if(bindResult == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d\n" , WSAGetLastError());
		closeServer();
	}
	listen(mySocket, SOMAXCONN);

	//開啟server
	while(true) 
	{
		clearBuffer(receiveBuffer);
		cout << endl << "Waitting for connection..." << endl;
		int receiveLength = recvfrom(mySocket, receiveBuffer, bufferLength, 0, (struct sockaddr*) &client, &addrlen);

		if(receiveLength == SOCKET_ERROR)
		{
			cout << "receive failed" << endl;
			continue;
		} 
		else if (receiveLength > 0)
            cout << "Bytes received: " << receiveLength << endl;

		cout << "Connecting ip: "<< inet_ntoa(client.sin_addr) << endl;
		cout << "Connecting ip's port: "<< ntohs(client.sin_port) << endl;
		FILE* fptr = fopen("Readme.txt", "rb");
		if(fptr != 0) 
		{
		int transformedBytes = 0;

		
		char *fileName = (char*) malloc(sizeof(char) * 64);
		strcpy(fileName, "Filename: ./test.txt");
		sendto(mySocket, fileName, strlen(fileName), 0, (struct sockaddr*) &client, addrlen);

		cout << "Send start." << endl;
		//傳送檔案資料 
		while(feof(fptr) == 0)
		{
			clearBuffer(fileOpenBuffer);
			int readBytes = fread(fileOpenBuffer, sizeof(char), bufferLength, fptr);
			transformedBytes += readBytes;
			cout << "Send data length: " << readBytes << endl;
			sendto(mySocket, fileOpenBuffer, readBytes, 0, (struct sockaddr*) &client, addrlen);
		}
		fclose(fptr);
		cout << "Total send data bytes: " << transformedBytes << endl;
		cout << "Data transfer over" << endl;
		sendto(mySocket, "Over", strlen("Over"), 0, (struct sockaddr*) &client, addrlen);
		} 
		else
		{
			cout << "Cannot open file." << endl;
			sendto(mySocket, "Error", strlen("Error"), 0, (struct sockaddr*) &client, addrlen);
		}
		cout << "Close connection" << endl << endl;
	}
	//Server關閉 
	if(mySocket > 0)
	{
		shutdown(mySocket, SD_BOTH);
		closesocket(mySocket);
	}
	WSACleanup();
	exit(-1);
	return 0;
}




