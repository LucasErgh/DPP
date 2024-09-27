#ifndef LISTENER_H
#define LISTENER_H

#include "ServerLogic.h"

#include <chrono>
#include <iostream>
#include <vector>
#include <queue>

#include <thread> // threads
#include <condition_variable> // allows you wait until a condition is met
#include <mutex> // allows you to lock mutually exclusive flag
#include <atomic> // used for single variables to prevent race conditions

// network socket stuff
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

namespace MySockets {

	class ClientHandler {
	private:
		SocketManager parent;
		SOCKET client;

	public:

		ClientHandler(SocketManager parent, SOCKET client) : parent(parent), client(client) {}

		// Attempts to connect to socket
		bool connect(SOCKET cSocket);

		// Tries to close connection
		bool disconnect();

		// Waits for client to respond after connecting and receiving orders
		void clientResponse();

	};

	class Listener
	{
	private:

		std::atomic<bool> keepListening = true;

		SOCKET ListenSocket;
		SocketManager parent;

	public:
		
		// 

		// Creates listner
		Listener(bool& succeeded);

		// start listening
		void startlistening();
	};

	class SocketManager {
	private:
		// represents whether the lisntener is active
		bool active = false;

		// mutex to make sure no two threads access workers at the same time
		std::mutex workerMtx;
		
		// condition to be notified by workers
		std::condition_variable cv;
		
		Listener listener;
		std::vector<ClientHandler> workers;
		std::atomic<ServerLogic> parent;

		// used by destructor, closes all connections client and listner
		void closeConnections();

	public:
		// default constructor
		SocketManager();
		
		// destructor, calls closeConnections
		~SocketManager();
		
		// Remove particular client, typically called by ClientHandler
		void removeClient(ClientHandler* client);
		// adds a new client to workers list, typically called by listner
		void addClient(SOCKET socket);

		// This is called by clients when they need new orders
		std::pair<int, int> requestOrders();
		// this is called to populate the order queue
		void getOrders();
	};

}

#endif