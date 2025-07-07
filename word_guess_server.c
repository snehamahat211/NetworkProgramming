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
