#include "log.hpp"

#include <iostream>
#include <fstream>

namespace Log {
    std::ofstream log_file("log.txt");

    void init() {
        if (!log_file) {
            std::cerr << "Error opening 'log.txt'" << std::endl;
            exit(1);
        }
    }

    void close() {
        log_file.close();
    }

    void info(const char* str) {
        log_file << "INFO :: " << str << std::endl;
    }

    void warn(const char* str) {
        log_file << "INFO :: " << str << std::endl;
    }

    void err(const char* str) {
        log_file << "INFO :: " << str << std::endl;
    }

    void fatal(const char* str) {
        log_file << "INFO :: " << str << std::endl;
        log_file.close();

        exit(1);
    }
}