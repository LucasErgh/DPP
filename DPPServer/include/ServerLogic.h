#pragma once

#include "MySockets.h"

#include <stdexcept>

#include<deque>
#include<set>
#include<fstream>

namespace PrimeProcessor{
typedef std::pair<int, int> range;
typedef std::vector<range> ranges;
typedef std::unique_ptr<SocketManager> Manager;

    class ServerLogic{
    private:
        const std::string rangeFile = "Ranges_Searched.txt";
        const std::string primeFile = "Primes_Found.txt";
        std::fstream rangesSearched;
        std::fstream primesFound;

        Manager manager;

        std::deque<range> workQueue;
        std::mutex workQueueMutex;

        ranges primesSearched;
        std::mutex primesSearchedMutex;

        std::set<int> primes;
        std::mutex primesMutex;

        // Stores set of primes on drive
        // Called when Prime set gets too big or the server is closing
        void storePrimes(std::set<int> p);
        
        // populate workQueue when it gets low
        void populateWorkQueue();

    public:
        ServerLogic();
        ~ServerLogic();

        bool start();
        void stop();

        void foundPrimes();
    };

}