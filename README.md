# Low Latency C++ Socket Communication

This project provides a low-latency C++ socket-based communication system featuring a multithreaded server and a responsive client application for real-time messaging.

## Features
- Multithreaded server supporting multiple clients.
- Real-time message broadcasting with low latency.
- Optimized networking using non-blocking I/O and select-based monitoring.
- Efficient connection management with error handling.

## Requirements
- Windows OS
- Microsoft Visual Studio or a compatible C++ compiler
- Winsock2 library (included in Windows SDK)

## How to Run
### Step 1: Compile the Server
1. Open the server code in your C++ IDE.
2. Compile the server code to generate the executable.
3. Run the server executable. It will start listening on port `9909`.

### Step 2: Compile the Client
1. Open the client code in your C++ IDE.
2. Compile the client code to generate the executable.
3. Run the client executable and connect to the server using IP `127.0.0.1` and port `9909`.

### Step 3: Test Communication
1. Start the server first and then run one or more clients.
2. Type a message in the client console to send it to the server.
3. Messages will be broadcast to all connected clients.

## Project Structure
- **server.cpp**: Contains the multithreaded server code.
- **client.cpp**: Contains the client application code.

## License
This project is licensed under the MIT License.

## Contact
For any issues or suggestions, please open an issue in the repository or contact the project maintainer.
