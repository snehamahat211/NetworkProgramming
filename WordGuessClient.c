#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ctype.h>
#include <windows.h> 
#pragma comment(lib, "ws2_32.lib") 
#define PORT 8080          
#define BUFFER_SIZE 1024 

void clear_screen() {
    system("cls");
}
int main() {
    WSADATA wsa;                      
    SOCKET client_socket;             
    struct sockaddr_in server_addr;   
    char buffer[BUFFER_SIZE]; 
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    printf("WSAStartup failed. Error Code: %d\n", WSAGetLastError());
    return 1; } 


    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) 
    {
    printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
    WSACleanup();
    return 1;}

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Connection failed. Error Code: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }
    printf("Connected to server! Starting game...\n\n");
    Sleep(1000);  
    while (1) {
        clear_screen();
        memset(buffer, 0, BUFFER_SIZE);
        if (recv(client_socket, buffer, BUFFER_SIZE, 0) <= 0) {
            printf("Server disconnected.\n");
            break;
        }
        printf("%s", buffer);

        if (strstr(buffer, "won") || strstr(buffer, "lost")) {
            break;
        }
        printf("\nEnter a letter: ");
        char guess;
        scanf(" %c", &guess);
        getchar(); 
        guess = tolower(guess);
        send(client_socket, &guess, 1, 0);
    }
        printf("\nPress any key to exit...");
        getchar();  // Wait before closing
        closesocket(client_socket);
        WSACleanup();
        return 0;
}





