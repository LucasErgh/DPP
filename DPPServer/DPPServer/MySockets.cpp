#include "MySockets.h"
#include "config.h"

namespace MySockets {

    /*------------------------------------------------------------------------------------------------------------------------------*/
    // ClientManager Methods



    /*------------------------------------------------------------------------------------------------------------------------------*/
    // Listner Methods

    Listener::Listener(bool& succeeded) {
        // Initialize Winsock
        WSADATA wsaData; // create object to hold windows sockets implementation
        int iResult;
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            std::cout << "WSAStartup failed: \n" << iResult;
            succeeded = false;
            return;
        }

        // Initialize socket object
        addrinfo* result = NULL, * ptr = NULL, hints;

        ZeroMemory(&hints, sizeof(hints)); // this is a macro in WinBase.h that fills a block of memory to zeros
        hints.ai_family = AF_INET; // specifies IPv4 address
        hints.ai_socktype = SOCK_STREAM; // specifies a stream socket
        hints.ai_protocol = IPPROTO_TCP; // specifies were using TCP protocol
        hints.ai_flags = AI_PASSIVE; // indicates we intend to use the returned socket address in a call to the bind function

        iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result); // resolve the local addreess and port to be used by the server
        if (iResult != 0) {
            std::cout << "getaddrinfo failed:\n" << iResult;
            WSACleanup();
            succeeded = false;
            return;
        }

        // Create a socket object called listensocket for the server to listen for client connections
        SOCKET ListenSocket = INVALID_SOCKET;

        // Call socket function and set it to ListenSocket, this creatse a socket that is bound toa specific transport service provider protocal (TCP in this case)
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

        if (ListenSocket == INVALID_SOCKET) {
            std::cout << "Error at socket():\n", WSAGetLastError();
            freeaddrinfo(result);
            succeeded = false;
            return;
        }

        // Bind to a socket

    // Setup the TCP listening socket
        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            printf("bind failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(ListenSocket);
            succeeded = false;
            return;
        }

        freeaddrinfo(result); // the information from getaddrinfo is no longer need once bind is called
        succeeded = true;
    }

    void Listener::startlistening() {
        bool succeeded = true;
        SOCKET ClientSocket = INVALID_SOCKET;

        do {
            // Startlistening
            // SOMAXCONN is a constant that instructs winsock to allow a maximum reasonable number of pending connections in queue
            if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
                std::cout << "Listen failed with error: %ld\n" << WSAGetLastError();
                succeeded = false;
            }

            // accept a client socket
             // temporary socket object called for accepting connections from 

            ClientSocket = accept(ListenSocket, NULL, NULL);
            if (ClientSocket == INVALID_SOCKET) {
                std::cout << "accept failed:\n" << WSAGetLastError();
                succeeded = false;
            }

            // make socket manager add new client to client list
            parent.addClient(ClientSocket);

            
        } while (succeeded);

        throw(1);

        return;
    }

    /*------------------------------------------------------------------------------------------------------------------------------*/
    // SocketManager Methods

    // used by destructor, closes all connections client and listner
    void SocketManager::closeConnections() {

    }

    // default constructor
    SocketManager::SocketManager() {
        // TO-DO figure out why this is an error
        listener = Listener(active);


    }

    // destructor, calls closeConnections
    SocketManager::~SocketManager() {
       // TO-DO 
    }

    // Remove particular client, typically called by ClientHandler
    void SocketManager::removeClient(ClientHandler* client) {

    }

    // adds a new client to workers list, typically called by listner
    void SocketManager::addClient(SOCKET socket) {
        // locking my worker mutex so no funny buisness starts happening
        std::lock_guard<std::mutex> lock(workerMtx);

        // create ClientHandler object



        workers.push_back(socket);


    }

    // This is called by clients when they need new orders
    std::pair<int, int> SocketManager::requestOrders() {

    }

    // this is called to populate the order queue
    void SocketManager::getOrders() {

    }

    /*------------------------------------------------------------------------------------------------------------------------------*/

}