// Source of information: https://msdn.microsoft.com
// Developed by Korvux Neb 

#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

#pragma comment(lib, "Ws2_32.lib")


int main(int argc, char *argv[])
{
	// Variables used to send data to the server.

	WSADATA wsaData;	// The WSADATA structure contains information about the Windows Sockets implementation.
	int iResult;		// Variable used to checks error in functions.
	SOCKADDR_IN SockAddr;	// Struct used by Winsocket to specify an local endpoint address or remote socket connection.
	std::string url = "httpbin.org";	// Target URL
	std::string request = "GET /user-agent HTTP/1.1\r\nHost: " + url + "\r\nUser-Agent: C++ Elite\r\n\r\n";	// Request that will be sended to Target URL
	struct hostent *host;	// hostent structure is used by functions to store information about a given host, such as host name, IPv4 address, and so forth.

	std::cout << "### SOCKET PROGRAMMING ###" << std::endl;

	// Initialize Winsock
	// Variable that will contain the result of WSAStartup, used to check if it has been successfully executed.s
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);		// The WSAStartup function initiates use of the Winsock DLL by a process.
	if (iResult != 0) {		// If successful, the WSAStartup function returns zero. 
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	SOCKET Socket;		// In Winsock applications, a socket descriptor IS NOT a file descriptor and must be used with the Winsock functions. 
	Socket = socket(AF_INET, SOCK_STREAM, 0);	// The socket function creates a socket that is bound to a specific transport service provider.
	if (Socket == INVALID_SOCKET) {		// If the socket call fails, it returns INVALID_SOCKET.
		std::cout << "Socket creation failed." << std::endl;
	}
	
	// The addrinfo structure is used by the getaddrinfo function to hold host address information.
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	SecureZeroMemory(&hints, sizeof(hints));	// Fills a block of memory with zeros.
	hints.ai_family = AF_INET;		// IPV4.
	hints.ai_socktype = SOCK_STREAM;	// Supports reliable connection-oriented byte stream communication.
	hints.ai_protocol = IPPROTO_TCP;	// Transmission control protocol aka TCP.

	// The getaddrinfo function provides protocol-independent translation from an ANSI host name to an address.

	#define DEFAULT_PORT "80"

	iResult = getaddrinfo(url.c_str(), DEFAULT_PORT, &hints, &result);	// Success returns zero. Failure returns a nonzero Windows Sockets error code
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();	// The WSACleanup function terminates use of the Winsock 2 DLL(Ws2_32.dll).
		return 1;
	}

	SockAddr.sin_port = htons(80); // The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).
	SockAddr.sin_family = AF_INET;	
	// sin_addr is the IP address in the socket | is a union, so it can be accessed as s_addr(one 4 - bytes integer).
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

	if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
		std::cout << "Could not connect";
		system("pause");
		//return 1;
	}

	// send GET / HTTP
	send(Socket, request.c_str(), strlen(request.c_str()), 0);
	
	int nDataLength;
	char buffer[10000];
	std::string response;
	// recieve html
	while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0) {
		int i = 0;
		while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {

			response += buffer[i];
			i += 1;
		}
	}

	closesocket(Socket);
	WSACleanup();

	// Display HTML source 
	std::cout << response.c_str() << std::endl;

	char input{ ' ' };	
	std::cin >> input;
    return 0;
}

