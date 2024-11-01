#pragma once

#include "MySockets.h"

#include <stdexcept>

#include<deque>
#include<set>
#include<fstream>

namespace PrimeProcessor{

typedef unsigned long long int ull;
typedef std::pair<ull, ull> range;
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
        ranges WIPQueue;
        std::mutex WIPQueueMutex;

        ranges primesSearched;
        std::mutex primesSearchedMutex;
        ull largestSearched;

        std::set<ull> primes;
        std::mutex primesMutex;

        // Stores set of primes on drive
        // Called when Prime set gets too big or the server is closing
        void storePrimes();
        
        // populate workQueue when it gets low
        void populateWorkQueue();

        // called after ranges searchd text file is read and primesSearched is populated
        // sorts the vector, merges sequential ranges, then adds missing ranges to workQueue
        void searchedNormalization();

    public:
        ServerLogic();
        ~ServerLogic();

        bool start();
        void stop();

        void foundPrimes(std::set<ull> primes);
    };

}