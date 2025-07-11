#include "application_state.hpp"
#include "constants.hpp"
#include "log.hpp"

#include <exception>
#include <format>
#include <fstream>
#include <string>

ApplicationState::ApplicationState() {
    std::ifstream in_file(SAVE_FILE);

    if (!in_file) {
        Log::err("could not open SAVE_FILE, resetting.");
        return;
    }

    std::string line;
    while(getline(in_file, line)) {
        std::size_t comma_index = 0;
        for(; comma_index < line.length(); ++comma_index) {
            if (line[comma_index] == ',') break;
        }

        LevelData ld;

        try {
            ld.moves = std::stoi(line.substr(0, comma_index));
            Log::debug(std::format("app-state :: load moves={}", ld.moves).c_str());
        } catch (std::exception e) {
            Log::debug(line.substr(0, comma_index).c_str());
            Log::fatal("corrupted save file for moves. Please remove and restart.");
        }

        try {
            ld.seconds_played= std::stod(line.substr(comma_index+1, line.length() - comma_index - 1));
            Log::debug(std::format("app-state :: load seconds_played={}", ld.seconds_played).c_str());
        } catch (std::exception e) {
            Log::debug(line.substr(comma_index+1, line.length() - comma_index).c_str());
            Log::fatal("corrupted save file for seconds_played. Please remove and restart.");
        }

        level_data.push_back(ld); // copy, bad
    }

    in_file.close();
}

ApplicationState::~ApplicationState() {
    save();
}

void ApplicationState::save() {
    std::ofstream out_file(SAVE_FILE);

    if (!out_file) {
        Log::err("Could not write to SAVE_DATA.");
        return;
    }

    for(LevelData& ld : level_data) {
        out_file << ld.moves << "," << ld.seconds_played << std::endl;
    }

    out_file.close();
}