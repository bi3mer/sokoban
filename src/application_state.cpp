#include "application_state.hpp"
#include "constants.hpp"
#include "log.hpp"

#include <exception>
#include <fstream>

ApplicationState::ApplicationState() {
    std::ifstream in_file(SAVE_FILE);

    if (!in_file) {
        Log::err("could not open SAVE_FILE, resetting.");
        return;
    }

    std::string data;
    getline(in_file, data);

    try {
        max_level_beaten = std::stoi(data);
    } catch(std::exception e) {
        Log::err(e.what());
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

    out_file << max_level_beaten;
    out_file.close();
}