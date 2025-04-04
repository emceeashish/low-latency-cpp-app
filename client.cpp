#include<bits/stdc++.h> // Includes all standard C++ libraries
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

#define PORT 9909
#define SIZE 1024

using namespace std;

SOCKET nClientSocket;
struct sockaddr_in srv;
fd_set readfds;
char buff[SIZE] = { 0 };
int nRet = 0;
bool isRunning = true;

// Clean up Winsock and socket
void cleanup() {
    closesocket(nClientSocket);
    WSACleanup();
    cout << "Cleaned up resources." << endl;
}
// Thread function to handle receiving messages from server
void handle_messages() {
    while (isRunning) {
        FD_ZERO(&readfds);
        FD_SET(nClientSocket, &readfds);

        // Set timeout to 1 second for select()
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int activity = select(0, &readfds, NULL, NULL, &timeout);

        if (activity < 0) {
            cerr << "select() failed." << endl;
            isRunning = false;
            break;
        }

        if (activity == 0) {
            // Timeout: No activity
            continue;
        }

        if (FD_ISSET(nClientSocket, &readfds)) {
            memset(buff, 0, SIZE);
            nRet = recv(nClientSocket, buff, SIZE - 1, 0);
            if (nRet <= 0) {
                cout << "Connection closed by server." << endl;
                isRunning = false;
                break;
            }
            cout << "Server: " << buff << endl;
        }
    }
}
// Thread function to send messages to the server
void send_message() {
    while (isRunning) {
        memset(buff, 0, SIZE);
        cin.getline(buff, SIZE);

        nRet = send(nClientSocket, buff, strlen(buff), 0);
        if (nRet == SOCKET_ERROR) {
            cerr << "send() failed with error: " << WSAGetLastError() << endl;
            isRunning = false;
            break;
        }
    }
}

int main() {
     // Initialize Winsock
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
        cerr << "WSAStartup failed." << endl;
        return EXIT_FAILURE;
    }

    nClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nClientSocket == INVALID_SOCKET) {
        cerr << "socket() failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Set socket options for low latency
    // Disable Nagle's algorithm for low latency (TCP_NODELAY)
    int flag = 1;
    if (setsockopt(nClientSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag)) < 0) {
        cerr << "setsockopt(TCP_NODELAY) failed." << endl;
        cleanup();
        return EXIT_FAILURE;
    }

    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(&srv.sin_zero, 0, sizeof(srv.sin_zero));

    nRet = connect(nClientSocket, (struct sockaddr*)&srv, sizeof(srv));
    if (nRet == SOCKET_ERROR) {
        cerr << "connect() failed with error: " << WSAGetLastError() << endl;
        cleanup();
        return EXIT_FAILURE;
    }

    cout << "Connected to the server." << endl;

    // Receive initial message from the server
    memset(buff, 0, SIZE);
    nRet = recv(nClientSocket, buff, SIZE - 1, 0);
    if (nRet <= 0) {
        cerr << "Failed to receive message from server or connection closed." << endl;
        cleanup();
        return EXIT_FAILURE;
    }
    cout << "Server: " << buff << endl;

    // Start threads for sending and receiving messages
    thread receiver(handle_messages);
    thread sender(send_message);

    sender.join();
    isRunning = false; // Stop the receiver thread when the sender thread exits
    receiver.join();

    cleanup();
    return 0;
}
