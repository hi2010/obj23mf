#pragma once


class SimpleLogger {
public:

    struct logMsg {
        int severity;
        std::string msg;
    };

    // use 0 as nothing to 9 as everything / severity level (high big number, low small number)
    // 9 is fails, 0 ist just some status and 4 or so is x result
    int logLevel = 0;
    bool printLogs = true;
    std::vector<logMsg> logList;

    void log(int severity, std::string msg) {
        struct logMsg lm = { severity, msg };
        this->logList.push_back(lm);
        if (printLogs && severity >= this->logLevel) {
            std::cout << "[" << severity << "]: " << msg << std::endl;
        }
    }
};
SimpleLogger logger;