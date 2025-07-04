#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "constants.hpp"

inline int ss_get_data() {
    std::ifstream in_file(SAVE_FILE);

    if (!in_file) {
        return 0;
    }

    std::string data;
    getline(in_file, data);

    try {
        int num_data = std::stoi(data);
        in_file.close();
        return num_data;
    } catch(...) {
        std::cerr << "Could not read data in " << SAVE_FILE << std::endl;
        std::cerr << "Delete the file or uncorrupt it." << std::endl;
        return 1;
    }

    return 0;
}

inline void ss_save_data(int data) {
    std::ofstream out_file(SAVE_FILE);

    if (!out_file) {
        std::cerr << "Encountered an error when opening " << SAVE_FILE << " and data was not stored.";
        exit(1);
    }

    out_file << data;
    out_file.close();
}