#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "sokoban.hpp"
#include "state.hpp"
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

    Sokoban sokoban;
    sokoban_init_from_level(sokoban, level);

    ui_init();

    State state = State::Menu;

    // Game loop
    char user_input;
    ui_render_menu();
    while (!sokoban_game_over(sokoban)) {
        user_input = ui_get_char();

        switch (state) {
            case State::Menu:
                if (user_input == '\n') {
                    state = State::Game;
                    ui_render_sokoban(sokoban);
                } else {
                    ui_render_menu();
                }
                break;
            case State::Game:
                sokoban_update(sokoban, user_input);
                ui_render_sokoban(sokoban);
                break;
        }
    }

    ui_close();

    free(sokoban.blocks);
    free(sokoban.solids);
    free(sokoban.switches);

    return 0;
}