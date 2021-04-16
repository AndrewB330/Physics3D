#include <common/logging.hpp>
#include <algorithm>

std::vector<Log> Logger::GetLastLogs(int number_of_logs) {
    std::vector<Log> res;
    for(int i = 0; i < number_of_logs && i < logs_list.size(); i++) {
        res.push_back(logs_list[logs_list.size() - i - 1]);
    }
    std::reverse(res.begin(), res.end());
    return res;
}

Logger &Logger::Instance() {
    static Logger instance;
    return instance;
}

std::string Logger::PrefixByType(LogType type) const {
    switch(type) {
        case LogType::LOG_INFO:
            return "[INFO]";
        case LogType::LOG_WARNING:
            return "[WARN]";
        case LogType::LOG_ERROR:
            return "[ERRO]";
    }
    return "[____]";
}
