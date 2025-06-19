#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "sokoban.hpp"
#include "ui.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "A path to a single Sokobon level must be provided." << std::endl;
        return 1;
    }

    std::ifstream level_file(argv[1]);
    if (!level_file.good()) {
        std::cerr << "Could not open file: " << argv[1] << std::endl;
        return 1;
    }

    std::vector<std::string> level;
    std::string line;
    while(getline(level_file, line)) {
        level.push_back(line);
    }

    level_file.close();

    const std::size_t num_columns = level[0].size();
    for(std::size_t i = 1; i < level.size(); ++i) {
        assert(level[i].size() == num_columns);
    }

    Sokoban state;
    state_init_from_level(state, level);

    ui_init();

    // Game loop
    ui_render_sokoban(state);
    while (!state_game_over(state)) {
        const char user_input = ui_get_char();
        state_update(state, user_input);
        ui_render_sokoban(state);
    }

    ui_close();


    free(state.blocks);
    free(state.solids);
    free(state.switches);

    return 0;
}