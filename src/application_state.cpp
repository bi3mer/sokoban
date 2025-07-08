#include "application_state.hpp"
#include "constants.hpp"
#include "log.hpp"

#include <algorithm>
#include <exception>
#include <fstream>
#include <cstdlib>
#include <sstream>

ApplicationState::ApplicationState() {
    std::ifstream in_file(SAVE_FILE);

    if (!in_file) {
        Log::warn("No save file found.");
        return;
    }

    std::string line;
    std::string data;
    while(getline(in_file, line)) {
        LevelData ld;
        std::stringstream ss(line);

        // moves
        getline(ss, data, ',');
        try {
            ld.moves = std::stoi(data);
        } catch(const std::exception& exception) {
            Log::fatal(exception.what());
        }

        // time
        getline(ss, data, ',');
        try {
            ld.seconds = std::stod(data);
        } catch(const std::exception& exception) {
            Log::fatal(exception.what());
        }

        level_data.emplace_back(std::move(ld));
    }

    in_file.close();
    level_id = level_data.size() - 1;
}

void ApplicationState::update(const LevelData& new_data) {
    if (level_id >= level_data.size()) {
        level_data.emplace_back(std::move(new_data));
        return;
    }

    level_data[level_id].moves = std::min(
        level_data[level_id].moves,
        new_data.moves
    );

    level_data[level_id].seconds = std::min(
        level_data[level_id].seconds,
        new_data.seconds
    );
}

void ApplicationState::save() const {
    std::ofstream out_file(SAVE_FILE);

    if (!out_file) {
        // @TODO: add a quit function
        exit(1);
    }

    for (const LevelData& ld : level_data) {
        out_file << ld.moves << "," << ld.seconds << std::endl;
    }

    out_file.close();
}