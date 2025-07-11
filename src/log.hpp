#pragma once

namespace Log {
    void init();
    void close();

    void info(const char*);
    void debug(const char*);
    void warn(const char*);
    void err(const char*);
    void fatal(const char*);
}