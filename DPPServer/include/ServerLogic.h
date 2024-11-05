#ifndef ServerLogic_h
#define ServerLogic_h

#include <fstream>
#include <stdexcept>

#include <memory>
#include <vector>
#include <deque>
#include <set>

#include <mutex>
#include <thread>

namespace PrimeProcessor{

    class SocketManager;

    class ServerLogic{
    private:
        const std::string rangeFile = "Ranges_Searched.txt";
        const std::string primeFile = "Primes_Found.txt";
        std::fstream rangesSearched;
        std::fstream primesFound;

        std::unique_ptr<SocketManager> manager;

        std::deque<std::array<unsigned long long, 2>> workQueue;
        std::mutex workQueueMutex;
        std::vector<std::array<unsigned long long, 2>> WIPQueue;
        std::mutex WIPQueueMutex;

        std::vector<std::array<unsigned long long, 2>> primesSearched;
        std::mutex primesSearchedMutex;
        unsigned long long largestSearched;

        std::set<unsigned long long> primes;
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

        // called by the server manager to get the next range to search
        std::array<unsigned long long, 2> getRange();

        // called by the server manager to return ranges found
        void foundPrimes(std::vector<unsigned long long> primes);
    };

}

#endif