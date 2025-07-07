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

