#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ctype.h>
#pragma comment(lib, "ws2_32.lib") 
#define PORT 8080          
#define BUFFER_SIZE 1024  
int main() {
    WSADATA wsa;                      
    SOCKET client_socket;             
    struct sockaddr_in server_addr;   
    char buffer[BUFFER_SIZE]; 
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    printf("WSAStartup failed. Error Code: %d\n", WSAGetLastError());
    return 1; } 
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
    printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
    WSACleanup();
    return 1;}
     server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");



