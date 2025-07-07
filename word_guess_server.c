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

    char guessed_word[strlen(secret_word) + 1];
    memset(guessed_word, '_', strlen(secret_word));
    guessed_word[strlen(secret_word)] = '\0';

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
                     "Current word: %s\nAttempts left: %d\n",
                     guessed_word, attempts_left);
        }
        send(client_socket, buffer, strlen(buffer), 0);
    }

    closesocket(client_socket);
}

// Main function to set up server and accept client connections
int main() {