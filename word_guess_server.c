// Author: Astha Thapa
// Project: WordGuessServer
// Description: Word guessing game server using Winsock (TCP)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Link Winsock library

#define PORT 8080
#define MAX_ATTEMPTS 6
#define BUFFER_SIZE 1024
#define WORD_COUNT 5

// ASCII Hangman stages
const char *hangman_stages[MAX_ATTEMPTS + 1] = {
    "  ___\n |    |\n |    \n |    \n |    \n_|",
    "  ___\n |    |\n |    O\n |    \n |    \n_|",
    "  ___\n |    |\n |    O\n |    |\n |    \n_|",
    "  ___\n |    |\n |    O\n |   /|\n |    \n_|",
    "  ___\n |    |\n |    O\n |   /|\\\n |    \n_|",
    "  ___\n |    |\n |    O\n |   /|\\\n |   / \n_|",
    "  ___\n |    |\n |    O\n |   /|\\\n |   / \\\n_|"
};

// List of random words
const char *word_list[WORD_COUNT] = {
    "apple", "banana", "cherry", "dragon", "elephant"
};

//Randon word selection

const char *get_random_word() {
    return word_list[rand() % WORD_COUNT];
}


// Send current hangman ASCII art to the client
void send_hangman(int stage, SOCKET client_socket) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "\n%s\n", hangman_stages[stage]);
    send(client_socket, buffer, strlen(buffer), 0);
}

//Handling client main
void handle_client(SOCKET client_socket) {
    const char *secret_word = get_random_word();  // Select secret word
    printf("Secret word for client: %s\n", secret_word);  // Print on server side

    size_t word_len = strlen(secret_word);
    char *guessed_word = (char *)malloc(word_len + 1);
    if (!guessed_word) {
        printf("Memory allocation failed.\n");
        closesocket(client_socket);
        return;
    }
    memset(guessed_word, '_', word_len);
    guessed_word[word_len] = '\0';

    int attempts_left = MAX_ATTEMPTS;
    int hangman_stage = 0;
    char buffer[BUFFER_SIZE];

    // Send initial game state to client
    snprintf(buffer, BUFFER_SIZE,
             "Guess the word (%zu letters): %s\nAttempts left: %d\n%s\n",
             strlen(secret_word), guessed_word, attempts_left, hangman_stages[hangman_stage]);
    send(client_socket, buffer, strlen(buffer), 0);

    while (attempts_left > 0 && strcmp(guessed_word, secret_word) != 0) {
        memset(buffer, 0, BUFFER_SIZE);
        int recv_len = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (recv_len <= 0) {
            printf("Client disconnected or recv error.\n");
            break;
        }
        buffer[recv_len] = '\0';

        char guess = tolower(buffer[0]);  // Take first character, case-insensitive
        int correct_guess = 0;

        // Check if guess is in secret word and update guessed_word accordingly
        for (size_t i = 0; secret_word[i] != '\0'; i++) {
            if (secret_word[i] == guess && guessed_word[i] == '_') {
                guessed_word[i] = guess;
                correct_guess = 1;
            }
        }

        if (!correct_guess) {
            attempts_left--;
            hangman_stage++;
            send_hangman(hangman_stage, client_socket);
        }

        // Prepare response message based on game status
        if (strcmp(guessed_word, secret_word) == 0) {
            snprintf(buffer, BUFFER_SIZE, "You won! The word was: %s\n", secret_word);
        } else if (hangman_stage >= MAX_ATTEMPTS) {
            snprintf(buffer, BUFFER_SIZE, "You lost! The word was: %s\n", secret_word);
        } else {
            snprintf(buffer, BUFFER_SIZE,
                     "Word: %s\nAttempts left: %d\n", 
                     guessed_word, attempts_left);
        }
        send(client_socket, buffer, strlen(buffer), 0);
    }
    free(guessed_word);

    closesocket(client_socket);
}

// Main function to set up server and accept client connections
int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int addr_len = sizeof(client_addr);

    // Seed random number generator once here
    srand(time(NULL));

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Bind socket to port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Listen for clients
    if (listen(server_socket, 5) == SOCKET_ERROR) {
        printf("Listen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept client connection (only one client)
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Client connected!\n");
    handle_client(client_socket);

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
