# WordGuessServer

## Title: WordGuessServer – TCP-based Word Guessing Game using Winsock  
---
**Name:** Astha Thapa(221707)

**Name:** Sneha Mahat(221742)



## Introduction

This project implements a command-line word guessing game (similar to Hangman) using the C programming language and Winsock2 on Windows. It demonstrates fundamental networking concepts such as client-server communication, socket programming, and data exchange using the TCP protocol.


## Objectives:
1. Understand and implement basic TCP server-client communication using Winsock.
2. Develop a word guessing (Hangman) game as a console application.
3. Learn to handle real-time data transfer between server and client over sockets.
4. Explore how a server maintains game logic and interacts with a text-based client interface.



## Background Theory:

### Networking and Socket Programming

In computer networking, **socket programming** allows processes (running programs) to communicate over a network. A **socket** is an endpoint for sending or receiving data. The most common protocol used in socket programming is **TCP (Transmission Control Protocol)**, which ensures reliable and ordered delivery of messages between a client and a server.

### TCP Server-Client Architecture

In a **TCP server-client model**, the server waits for incoming connections, while the client initiates a request. Once the connection is established:
- The server can send/receive data from the client.
- The client can communicate using text, binary, or any agreed protocol.

### Winsock (Windows Sockets API)

**Winsock** is a Windows-specific API for socket programming. It allows developers to create server and client programs that can communicate using TCP or UDP protocols. It requires explicit initialization (using `WSAStartup`) and cleanup (`WSACleanup`) functions in a C program.

### Game Logic – Hangman

**Hangman** is a classic word guessing game where the player guesses letters in an unknown word. Each incorrect guess results in a part of a hangman figure being drawn. After a limited number of incorrect guesses (typically 6), the game ends. In this project, the server handles:
- Word selection
- Input processing
- Tracking attempts
- Displaying hangman stages using ASCII art



## Procedure:

### 1. Environment Setup
- Install a C compiler (e.g., MinGW or Visual Studio) on a Windows system.
- Ensure `ws2_32.lib` (Winsock library) is available for linking.

### 2.1. Writing the Server Code
- Create a file `WordGuessServer.c` and implement:
  - Winsock initialization (`WSAStartup`)
  - Socket creation and binding
  - Listening for a single client connection
  - Game logic for Hangman (word selection, guess checking, ASCII updates)
  - Cleanup (`closesocket`, `WSACleanup`)


### 2.2. Compiling the Program
- Compile using the command:
  ```bash
  gcc WordGuessServer.c -o WordGuessServer.exe -lws2_32

  ### 4. Running the Server
- Execute the compiled server program from the command line:
  ```bash
  WordGuessServer.exe
  
### 2.3. Running the Server
- Execute the compiled server program from the command line:
  ```bash
  WordGuessServer.exe

### 3.1. Writing the Server Code
- Create a new file named `WordGuessClient.c`.
- Implement the following functionalities:
  - Initialize Winsock using `WSAStartup`.
  - Create a socket with TCP protocol (`SOCK_STREAM`).
  - Set up server connection parameters (IP: `127.0.0.1`, Port: `8080`).
  - Attempt to connect to the server using `connect()`.
  - Wait for the server’s game instructions and display them.
  - Accept one-letter guesses from the user via keyboard input.
  - Send each guess to the server using `send()`.
  - Receive updated game state from the server via `recv()`.
  - Repeat the process until a win or loss message is received.
  - Properly handle socket closure and clean up resources using `closesocket()` and `WSACleanup()`.

### 3.2. Compiling the Client Program
- Use the following command to compile:
  ```bash
  gcc WordGuessClient.c -o WordGuessClient.exe -lws2_32

### 3.3. Running the Server
- Execute the compiled client program from the command line:
  ```bash
  WordGuessClient.exe


## Technologies Used

- **Programming Language:** C  
- **Networking API:** Winsock2  
- **Communication Protocol:** TCP  
- **Platform:** Windows OS  
- **Tools:** MinGW/Visual Studio  


## Conclusion

This project strengthened our understanding of **TCP socket programming** using **Winsock** in C. By designing both the server and client, we experienced:

- Real-time data exchange using sockets  
- Game loop design and logic separation  
- Handling input/output between different machines  

We successfully developed a working model of a **networked multiplayer game**, which can be extended into more advanced systems involving concurrent clients, graphical interfaces, or more complex logic. This lab helped us apply low-level networking concepts in a practical and interactive way.

---

## Authors

- Sneha Mahat  
- Astha Thapa
