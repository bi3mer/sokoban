#include <fstream>
#include<iostream>

namespace Log {
    std::ofstream log_file("log.txt");

    void init() {
        if (!log_file) {
            std::cerr << "FATAL :: unable to open log file: 'log.txt'" << std::endl;
            exit(1);
        }
    }

    void close() {
        log_file << "EOF";
        log_file.close();
    }

    void info(const char* str) {
        log_file << "INFO :: " << str << std::endl;
        log_file.flush();
    }

    void warn(const char* str) {
        log_file << "WARN :: " << str << std::endl;
        log_file.flush();
    }

    void err(const char* str) {
        log_file << "ERROR :: " << str << std::endl;
        log_file.flush();
    }

    void fatal(const char* str) {
        log_file << "FATAL :: " << str << std::endl;
        log_file.close();
        exit(1);
    }
}