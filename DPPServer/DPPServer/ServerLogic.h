#ifndef SERVERLOGIC_H
#define SERVERLOGIC_H

#include "MySockets.h"

#include <queue>

#include <thread>
#include <mutex>
#include <condition_variable> 
#include <atomic>
#include <chrono>

// ServerLogic Responsibilities:
//		- Creates and manages MySockets object
//		- Waits for console input to stop program
//		- gets primes found from MySockets
//		- Stores and orders primes somehow

class ServerLogic
{
private: 
	// list of assignments
	std::atomic<std::queue<std::pair<int, int>>> assignmentQueue;
	// Socket manager to interface with connections
	MySockets::SocketManager sockets;

	// starts listening and loop, called by constructor
	void start();

	// closes connections stops listening and frees winsock resources, called by destructor
	void stop();

public:
	// constructor 
	ServerLogic();
	

	// populates assignmentQueue 
	void getTasks();

	// called by socketmangaer when its workers return with primes found
	// this will store primes in a plaintext file for now
	void primeFound();

	// called by socketManager to get the next range for its workers
	std::pair<int, int> getNextRange();
};

#endif