#include <winsock2.h> // Windows Sockets API header
#include <ws2tcpip.h> // TCP/IP Protocol header
#pragma comment(lib, "Ws2_32.lib") // linker directive to link against Ws2_32.lib
#include <iostream> // C++ standard input/output library
#include <stdlib.h> // standard library header
#include <string.h> // string library header
using namespace std;

int main() {
    // Initialize Winsock
    WORD wVersionRequested; // variable to store the requested Winsock version
    WSADATA wsaData; // struct to store Winsock data
    wVersionRequested = MAKEWORD(2, 2); // set the requested Winsock version to 2.2
    WSAStartup(wVersionRequested, &wsaData); // initialize Winsock

    // Create socket
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0); // create a TCP socket
    // AF_INET: address family for IPv4, SOCK_STREAM: TCP protocol, 0: default protocol

    // Set server address and port
    struct sockaddr_in server; // struct to store the server address and port
    server.sin_family = AF_INET; // set address family to IPv4
    server.sin_port = htons(1280); // set port number to 1280
    InetPton(AF_INET, L"127.0.0.1", &server.sin_addr.s_addr); // convert IP address to binary format

    // Connect to server
    int c = connect(s, (struct sockaddr*)&server, sizeof(server)); // connect to server
    if (c == SOCKET_ERROR) { // check if connection failed
        cerr << "Could not connect to server\n"; // print error message
        WSACleanup(); // clean up Winsock
        return 1; // exit with error code
    }

    // Send two words to server
    cout << "Enter the first word: ";
    string word1;
    cin >> word1;
    cout << "Enter the second word: ";
    string word2;
    cin >> word2;
    send(s, word1.c_str(), word1.length(), 0); // send first word to server
    send(s, word2.c_str(), word2.length(), 0); // send second word to server
    cout << "Sent two words to the server: " << word1 << " " << word2 << endl;

    // Receive response from server
    char buffer[1024]; // define a buffer to store the received message
    c = recv(s, buffer, sizeof(buffer), 0); // receive a message from the server
    if (c == SOCKET_ERROR) { // check if there was an error in receiving the message
        cout << "recv failed with error: " << WSAGetLastError() << endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    buffer[c] = '\0'; // set the last byte to a null terminator to ensure the string ends properly
    cout << "Received result from the server: " << buffer << endl; // print the received message to the console

    // Close socket and clean up
    closesocket(s); // close socket
    WSACleanup(); // clean up Winsock

    return 0; // exit program with success code
}
