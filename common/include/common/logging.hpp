#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include <SDL_log.h>
#ifdef SDL_VERSION
#endif

enum class LogType {
    LOG_INFO = 0,
    LOG_WARNING = 1,
    LOG_ERROR = 2
};

struct Log {
    LogType type;
    std::string message;
};

class Logger {
public:
    template<class... Args>
    void AddLog(LogType type, std::string message, Args... args) {
        char buffer[1024];
        snprintf(buffer, 1024, message.c_str(), args...);
        std::string formatted = std::string(buffer);
        logs_list.push_back({type, formatted});


        switch(type) {
            case LogType::LOG_INFO:
                SDL_LogInfo(0, formatted.c_str());
                break;
            case LogType::LOG_WARNING:
                SDL_LogWarn(0, formatted.c_str());
                break;
            case LogType::LOG_ERROR:
                SDL_LogError(0, formatted.c_str());
                break;
        }
#ifdef SDL
#else
        std::cout << PrefixByType(type) << " " << logs_list.back().message << std::endl;
#endif
    }

    template<class... Args>
    void Info(std::string message, Args... args) {
        AddLog(LogType::LOG_INFO, message, args...);
    }

    template<class... Args>
    void Warning(std::string message, Args... args) {
        AddLog(LogType::LOG_WARNING, message, args...);
    }

    template<class... Args>
    void Error(std::string message, Args... args) {
        AddLog(LogType::LOG_ERROR, message, args...);
    }

    template<class... Args>
    static void LogInfo(std::string message, Args... args) {
        Instance().Info(message, args...);
    }

    template<class... Args>
    static void LogWarning(std::string message, Args... args) {
        Instance().Warning(message, args...);
    }

    template<class... Args>
    static void LogError(std::string message, Args... args) {
        Instance().Error(message, args...);
    }

    std::vector<Log> GetLastLogs(int number_of_logs = 16);

    static Logger & Instance();
private:
    std::string PrefixByType(LogType type) const;

    Logger() = default;

    std::vector<Log> logs_list;
};