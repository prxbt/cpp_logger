#include "Logger.h"
#include <thread>
#include <vector>

void logMessages(int threadId) {
    Logger& logger = Logger::getInstance();

    logger.debug("Debug message from thread " + std::to_string(threadId));
    logger.info("Info message from thread " + std::to_string(threadId));
    logger.warn("Warning message from thread " + std::to_string(threadId));
    logger.error("Error message from thread " + std::to_string(threadId));
}

int main() {
    Logger& logger = Logger::getInstance();
    
    logger.setLogLevel(LogLevel::DEBUG);
    
    // Optional: Set log output to a file
    logger.setLogFile("log.txt");

    // Create multiple threads to generate log messages
    const int numThreads = 5;
    std::vector<std::thread> threads;
    
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(logMessages, i);
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    return 0;
}

