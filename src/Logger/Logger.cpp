#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <vector>

using namespace std;

std::string CurrentDateTimeToString() {
    // This should be displayed in green and wiot
    time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    string output(30, '\0');
    strftime(&output[0], output.size(), "%Y-%b-%d %H:%M:%S", std::localtime(&now));
    return output;
}

namespace Logger {
    static std::vector<LogEntry> logs;

   void Log(const std::string& msg) {
       logs.push_back({.type = LOG_INFO, .msg = msg});
       std::string output = "LOG: [" + CurrentDateTimeToString() + "]: " + msg;
       cout << "\x1B[32m" << output << "\033[0m" << endl;
   }

   void Err(const std::string& msg) {
       logs.push_back({.type = LOG_ERROR, .msg = msg});
       std::string output = "LOG: [" + CurrentDateTimeToString() + "]: " + msg;
       cout << "\x1B[91m" << output << "\033[0m" << endl;
   }
}