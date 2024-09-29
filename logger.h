#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <memory>
#include <thread>
#include <chrono>
#include <ctime>
#include <sstream>

// Log Levels
enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    CRIT,
    ERROR
};

// Logger class
class Logger {
public:
    // Get the singleton instance of the Logger
    static Logger& getInstance() {
        static Logger instance; // Guaranteed to be destroyed. Instantiated on first use.
        return instance;
    }

    // Delete copy constructor and assignment operator to prevent copying the singleton
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Configure the logger to write to a file
    void setLogFile(const std::string& fileName) {
        std::lock_guard<std::mutex> lock(logMutex);
        if (fileStream.is_open()) {
            fileStream.close();
        }
        fileStream.open(fileName, std::ios::out | std::ios::app);
    }

    // Set log level for filtering messages
    void setLogLevel(LogLevel level) {
        std::lock_guard<std::mutex> lock(logMutex);
        logLevel = level;
    }

    // Log message function
    void log(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(logMutex);
        if (level >= logLevel) {
            std::string logMessage = formatLogMessage(level, message);

            // Write to console
            std::cout << logMessage << std::endl;

            // Write to file if file is set
            if (fileStream.is_open()) {
                fileStream << logMessage << std::endl;
            }
        }
    }

    // Utility log methods for specific log levels
    void debug(const std::string& message) {
        log(LogLevel::DEBUG, message);
    }

    void info(const std::string& message) {
        log(LogLevel::INFO, message);
    }

    void warn(const std::string& message) {
        log(LogLevel::WARN, message);
    }
    
   void crit(const std::string& message) {
        log(LogLevel::CRIT, message);
    }

    void error(const std::string& message) {
        log(LogLevel::ERROR, message);
    }

private:
    LogLevel logLevel = LogLevel::DEBUG;  // Default log level
    std::ofstream fileStream;             // Output file stream for logging
    std::mutex logMutex;                  // Mutex for thread safety

    Logger() {}  // Private constructor for Singleton pattern
    ~Logger() {
        if (fileStream.is_open()) {
            fileStream.close();
        }
    }

    // Helper function to format log messages
    std::string formatLogMessage(LogLevel level, const std::string& message) {
        std::stringstream ss;
        ss << "[" << currentDateTime() << "] "
           << "[" << logLevelToString(level) << "] "
           << "[" << std::this_thread::get_id() << "] "
           << message;
        return ss.str();
    }

    // Get current time as string
    std::string currentDateTime() {
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char buf[100];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return buf;
    }

    // Convert log level to string
    std::string logLevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARN: return "WARN";
            case LogLevel::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }
};

#endif // LOGGER_H

