#include <iostream>
#include <queue>
#include <winsock.h>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>

#define SIZE 1024
#define PORT 9909
#define MAX_CLIENTS 5

using namespace std;

struct sockaddr_in srv;
fd_set fr, fe;
int nSocket;
char buff[SIZE] = {0};
int ArrClient[MAX_CLIENTS] = {0}; // To handle multiple clients
queue<int> pendingConnections;   // Queue to hold pending connections
mutex queueMutex;                // Mutex for thread-safe queue operations
condition_variable cv;           // Condition variable for signaling threads

// Function prototypes
void processNewMessage(int nClientSocket);
void handlePendingConnections();
void clientDisconnected(int nClientSocket);
void broadcastMessage(const char* msg, int sender);

// Function to broadcast messages to all clients except the sender
void broadcastMessage(const char* msg, int sender) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (ArrClient[i] != 0 && ArrClient[i] != sender) {
            send(ArrClient[i], msg, strlen(msg), 0);
        }
    }
}

// Handle client disconnection
void clientDisconnected(int nClientSocket) {
    closesocket(nClientSocket);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (ArrClient[i] == nClientSocket) {
            ArrClient[i] = 0;
            break;
        }
    }
    cout << "Client disconnected: " << nClientSocket << endl;
}

// Process new messages from a client
void processNewMessage(int nClientSocket) {
    memset(buff, 0, SIZE);
    int nRet = recv(nClientSocket, buff, SIZE, 0);
    if (nRet <= 0) {
        if (nRet == 0 || WSAGetLastError() == WSAECONNRESET) {
            clientDisconnected(nClientSocket);
        }
    } else {
        cout << "Message from client " << nClientSocket << ": " << buff << endl;
        broadcastMessage(buff, nClientSocket);
    }
}

// Thread to handle pending connections
void handlePendingConnections() {
    while (true) {
        unique_lock<mutex> lock(queueMutex);
        cv.wait(lock, [] { return !pendingConnections.empty(); });

        while (!pendingConnections.empty()) {
            int clientSocket = pendingConnections.front();
            pendingConnections.pop();

            // Check if there's space for a new client
            bool spaceAvailable = false;
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (ArrClient[i] == 0) {
                    ArrClient[i] = clientSocket;
                    send(clientSocket, "Connected successfully.", 23, 0);
                    spaceAvailable = true;
                    break;
                }
            }

            if (!spaceAvailable) {
                send(clientSocket, "Server full. Connection denied.", 30, 0);
                closesocket(clientSocket);
            }
        }
    }
}

int main() {
    int nRet;
    WSADATA ws;

    // Initialize WSA
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
        cout << "WSA initialization failed." << endl;
        return EXIT_FAILURE;
    }

    // Create the server socket
    nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nSocket < 0) {
        cout << "Socket creation failed." << endl;
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Configure server address
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(nSocket, (sockaddr*)&srv, sizeof(srv)) < 0) {
        cout << "Failed to bind socket." << endl;
        closesocket(nSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Start listening
    if (listen(nSocket, 5) < 0) {
        cout << "Failed to listen on socket." << endl;
        closesocket(nSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    cout << "Server started. Waiting for connections..." << endl;

    // Start thread for handling pending connections
    thread connectionHandler(handlePendingConnections);
    connectionHandler.detach();

    while (true) {
        FD_ZERO(&fr);
        FD_ZERO(&fe);
        FD_SET(nSocket, &fr);
        int nMaxFd = nSocket;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (ArrClient[i] != 0) {
                FD_SET(ArrClient[i], &fr);
                FD_SET(ArrClient[i], &fe);
                if (ArrClient[i] > nMaxFd) {
                    nMaxFd = ArrClient[i];
                }
            }
        }

        nRet = select(nMaxFd + 1, &fr, nullptr, &fe, nullptr);
        if (nRet > 0) {
            if (FD_ISSET(nSocket, &fr)) {
                // Accept new connection
                int clientSocket = accept(nSocket, nullptr, nullptr);
                if (clientSocket >= 0) {
                    lock_guard<mutex> lock(queueMutex);
                    pendingConnections.push(clientSocket);
                    cv.notify_one();
                }
            }

            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (ArrClient[i] != 0 && FD_ISSET(ArrClient[i], &fr)) {
                    processNewMessage(ArrClient[i]);
                }
            }
        }
    }

    closesocket(nSocket);
    WSACleanup();
    return 0;
}
