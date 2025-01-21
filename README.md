# low-latency-cpp-app
<!DOCTYPE html>
<html>
<head>
    <title>Low Latency C++ Socket Communication</title>
</head>
<body>
    <h1>Low Latency C++ Socket Communication</h1>
    <p>This project provides a low-latency C++ socket-based communication system featuring a multithreaded server and a responsive client application for real-time messaging.</p>

    <h2>Features</h2>
    <ul>
        <li>Multithreaded server supporting multiple clients.</li>
        <li>Real-time message broadcasting with low latency.</li>
        <li>Optimized networking using non-blocking I/O and select-based monitoring.</li>
        <li>Efficient connection management with error handling.</li>
    </ul>

    <h2>Requirements</h2>
    <ul>
        <li>Windows OS</li>
        <li>Microsoft Visual Studio or a compatible C++ compiler</li>
        <li>Winsock2 library (included in Windows SDK)</li>
    </ul>

    <h2>How to Run</h2>
    <h3>Step 1: Compile the Server</h3>
    <pre>
1. Open the server code in your C++ IDE.
2. Compile the server code to generate the executable.
3. Run the server executable. It will start listening on port <strong>9909</strong>.
    </pre>

    <h3>Step 2: Compile the Client</h3>
    <pre>
1. Open the client code in your C++ IDE.
2. Compile the client code to generate the executable.
3. Run the client executable and connect to the server using IP <strong>127.0.0.1</strong> and port <strong>9909</strong>.
    </pre>

    <h3>Step 3: Test Communication</h3>
    <ul>
        <li>Start the server first and then run one or more clients.</li>
        <li>Type a message in the client console to send it to the server.</li>
        <li>Messages will be broadcast to all connected clients.</li>
    </ul>

    <h2>Project Structure</h2>
    <ul>
        <li><strong>server.cpp:</strong> Contains the multithreaded server code.</li>
        <li><strong>client.cpp:</strong> Contains the client application code.</li>
    </ul>

    <h2>License</h2>
    <p>This project is licensed under the MIT License.</p>

    <h2>Contact</h2>
    <p>For any issues or suggestions, please open an issue in the repository or contact the project maintainer.</p>
</body>
</html>
