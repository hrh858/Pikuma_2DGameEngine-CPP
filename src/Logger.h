#ifndef LOGGER_H
#define LOGGER_H

#include <string>

namespace Logger {
    enum LogEntryType {
        LOG_INFO,
        LOG_ERROR,
    };

    struct LogEntry {
        LogEntryType type;
        std::string msg;
    };

    void Log(const std::string& msg);
    void Err(const std::string& msg);
}

#endif
