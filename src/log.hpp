#pragma once

#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>

namespace Log {
    inline std::ofstream log_file("log.txt");

    inline void init() {
        if (!log_file) {
            std::cerr << "FATAL :: unable to open log file: 'log.txt'" << std::endl;
            std::abort();
        }
    }

    inline void close() {
        log_file << "EOF";
        log_file.close();
    }

    template <typename ...Args>
    inline void _log(const char* prefix, const char* str, Args&&... args) {
        if constexpr (sizeof...(args) > 0) {
            log_file << prefix << std::vformat(str, std::make_format_args(args...)) << std::endl;
        } else {
            log_file << prefix << str << std::endl;
        }

        log_file.flush();
    }

    template <typename ...Args>
    void debug(const char* str, Args&&... args) {
        _log("DEBUG :: ", str, std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void info(const char* str, Args&&... args) {
        _log("INFO :: ", str, std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void warning(const char* str, Args&&... args) {
        _log("WARNING :: ", str, std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void error(const char* str, Args&&... args) {
        _log("ERROR :: ", str, std::forward<Args>(args)...);
    }

    template <typename ...Args>
    [[noreturn]] void fatal(const char* str, Args&&... args) {
        _log("FATAL :: ", str, std::forward<Args>(args)...);
        std::abort();
    }
}