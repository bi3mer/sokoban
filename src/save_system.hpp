#pragma once

#include "constants.hpp"
#include <fstream>
#include <iostream>
#include <string>

inline int ss_get_data() {
    std::ifstream in_file(SAVE_FILE);

    if (!in_file) {
        return 0;
    }

    std::string data;
    getline(in_file, data);

    int num_data = 0;
    try {
        num_data = std::stoi(data);
    } catch(...) {
        // @TODO: should probably log to a file or something
    }

    in_file.close();

    return num_data;
}

inline void ss_save_data(int data) {
    std::ofstream out_file(SAVE_FILE);

    if (!out_file) {
        std::cerr << "Encountered an error when opening "
                  << SAVE_FILE
                  << " and data was not stored." << std::endl;

        exit(1);
    }

    out_file << data;
    out_file.close();
}