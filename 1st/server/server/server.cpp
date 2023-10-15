// Include necessary headers
#include <winsock2.h> // Windows Sockets 2 API header file
#include <ws2tcpip.h> // Windows Sockets 2 API header file for TCP/IP protocols
#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib library
#include <iostream> // Standard Input-Output stream header file
#include <stdlib.h> // Standard library header file

using namespace std; // Namespace for standard C++ library

int main() {
    // Initialize Winsock
    WORD wVersionRequested; // Version of Windows Sockets specification requested
    WSADATA wsaData; // Struct that receives information about the Windows Sockets implementation
    wVersionRequested = MAKEWORD(2, 2); // Request Windows Sockets version 2.2
    WSAStartup(wVersionRequested, &wsaData); // Call WSAStartup to initialize Winsock

    // Create socket
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);  // Create a socket with the AF_INET address family, SOCK_STREAM type, and default protocol
    if (s == INVALID_SOCKET) {  // If socket creation fails, print error message
        cout << "Failed to create socket\n";
        WSACleanup();  // Call WSACleanup to terminate use of Winsock DLL
        return 1;  // Return error code 1
    }

    // Bind socket to a local address
    sockaddr_in local;  // Struct that contains an IP address and port number
    local.sin_family = AF_INET;  // Set the address family to AF_INET (IPv4)
    local.sin_port = htons(1280);  // Set the port number to 1280 (in network byte order)
    local.sin_addr.s_addr = htonl(INADDR_ANY);  // Set the IP address to INADDR_ANY (allow incoming connections on any local interface)
    int c = bind(s, (sockaddr*)&local, sizeof(local));  // Bind the socket to the local address and port


    // Listen for incoming connections
    if (listen(s, SOMAXCONN) == SOCKET_ERROR) {  // Start listening on the socket for incoming connections
        cout << "Failed to listen\n" << WSAGetLastError() << endl;  // If listening fails, print error message
        closesocket(s);  // Call closesocket to close the socket
        WSACleanup();  // Call WSACleanup to terminate use of Winsock DLL
        return 1;  // Return error code 1
    }
    cout << "Server is listening on port 1280..." << endl;


    // Accept incoming connections
    SOCKET clientSocket = accept(s, NULL, NULL);  // Accept a connection on the socket and create a new socket for communication with the client
    if (clientSocket == INVALID_SOCKET) {  // If accepting connection fails, print error message
        cout << "Failed to accept connection\n";
        closesocket(s);  // Call closesocket to close the listening socket
        WSACleanup();  // Call WSACleanup to terminate use of Winsock DLL
        return 1;
    }
    cout << "Client connected!" << endl;


    // Receive the first word from the client
    char buffer[1024]; // create a buffer to store received data
    c = recv(clientSocket, buffer, sizeof(buffer), 0); // receive data from the client and store it in the buffer
    if (c == SOCKET_ERROR) { // if an error occurs during the receive operation
        std::cerr << "recv failed with error: " << WSAGetLastError() << std::endl; // output an error message and the error code
        closesocket(clientSocket); // close the socket connection with the client
        WSACleanup(); // clean up Winsock resources
        return 1; // exit the program with an error code
    }
    string word1(buffer, c); // convert the received data into a string and store it in the 'word1' variable

    // Receive the second word from the client
    c = recv(clientSocket, buffer, sizeof(buffer), 0); // receive data from the client and store it in the buffer
    if (c == SOCKET_ERROR) { // if an error occurs during the receive operation
        cout << "recv failed with error: " << WSAGetLastError() << endl; // output an error message and the error code
        closesocket(clientSocket); // close the socket connection with the client
        WSACleanup(); // clean up Winsock resources
        return 1; // exit the program with an error code
    }
    string word2(buffer, c); // convert the received data into a string and store it in the 'word2' variable
    cout << "Received two words from the client: " << endl;

    bool areEqual = (word1 == word2); // compare the two received words and store the result in the 'areEqual' variable

    // Send the result to the client
    const char* result = areEqual ? "true" : "false"; // convert the 'areEqual' boolean value into a string
    c = send(clientSocket, result, strlen(result), 0); // send the result back to the client
    if (c == SOCKET_ERROR) { // if an error occurs during the send operation
        cout << "send failed with error: " << WSAGetLastError() << endl; // output an error message and the error code
        closesocket(clientSocket); // close the socket connection with the client
        WSACleanup(); // clean up Winsock resources
        return 1; // exit the program with an error code
    }

    cout << "Sent the result to the client: " << result <<endl;

    // Close the connection
    closesocket(clientSocket); // close the socket connection with the client
    closesocket(s); // close the server socket
    WSACleanup(); // clean up Winsock resources

    return 0; // exit the program with a success code
}