#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "ncurses.h"

struct Point {
    int x;
    int y;
};

inline Point point_add(const Point p1, const Point p2) {
    return {
        .x = p1.x + p2.x,
        .y = p1.y + p2.y
    };
}

inline void set_uchar_bit(unsigned char* val, std::size_t index) {
    const std::size_t byte_index = index / 8;
    const std::size_t bit_position = index % 8;

    val[byte_index] |= (1 << bit_position);
}

inline void clear_uchar_bit(unsigned char* val, std::size_t index) {
    const std::size_t byte_index = index / 8;
    const std::size_t bit_position = index % 8;

    val[byte_index] &= ~(1 << bit_position);
}

inline bool get_uchar_bit(unsigned char* val, std::size_t index) {
    const std::size_t byte_index = index / 8;
    const std::size_t bit_position = index % 8;

    return (val[byte_index] >> bit_position) & 1;
}

struct State {
    std::size_t rows = 0;
    std::size_t columns = 0;
    Point player = { 0, 0 };

    unsigned char* blocks;
    unsigned char* solids;
    unsigned char* switches;
};

inline std::size_t state_index(const State& state, Point p) {
    return p.y * state.columns + p.x;
}

inline void state_set_block(State& state, Point p) {
    set_uchar_bit(state.blocks, state_index(state, p));
}

inline void state_clear_block(State& state, Point p) {
    clear_uchar_bit(state.blocks, state_index(state, p));
}

inline bool state_is_block(const State& state, Point p) {
    return get_uchar_bit(state.blocks, state_index(state, p));
}

inline void state_set_solid(State& state, Point p) {
    set_uchar_bit(state.solids, state_index(state, p));
}

inline bool state_is_solid(const State& state, Point p) {
    return get_uchar_bit(state.solids, state_index(state, p));
}

inline void state_set_switches(State& state, Point p) {
    set_uchar_bit(state.switches, state_index(state, p));
}

inline bool state_is_switch(const State& state, Point p) {
    return get_uchar_bit(state.switches, state_index(state, p));
}

void state_init_from_level(State& state, const std::vector<std::string>& level) {
    state.rows = level.size();
    state.columns = level[0].size();

    const std::size_t num_bytes = (state.rows * state.columns + 7) / 8;

    state.blocks = (unsigned char*) malloc(num_bytes);
    memset(state.blocks, 0, num_bytes);

    state.solids = (unsigned char*) malloc(num_bytes);
    memset(state.solids, 0, num_bytes);

    state.switches = (unsigned char*) malloc(num_bytes);
    memset(state.switches, 0, num_bytes);

    Point p;
    for (p.y = 0; p.y < state.rows; ++p.y) {
        const std::string& row = level[p.y];

        for (p.x = 0; p.x < state.columns; ++p.x) {
            switch (row[p.x]) {
                case ' ':
                    break;
                case 'X':
                    state_set_solid(state, p);
                    break;
                case 'b':
                    state_set_block(state, p);
                    break;
                case '.':
                    state_set_switches(state, p);
                    break;
                case 'B':
                    state_set_block(state, p);
                    state_set_switches(state, p);
                    break;
                case '@':
                    state.player = p;
                    break;
                default:
                    std::cerr << "Unnknown tile type: " << row[p.x] << std::endl;
                    assert(false);
            }
        }
    }
}

bool state_game_over(const State& state) {
    const std::size_t num_bytes = (state.rows * state.columns + 7) / 8;
    return !memcmp(state.switches, state.blocks, num_bytes);
}

void state_update(State& state) {
    const char user_input = getch();
    Point dir;
    switch (user_input) {
        case 'W':
        case 'w':
            dir = {0, -1};
            break;
        case 'A':
        case 'a':
            dir = {-1, 0};
            break;
        case 'S':
        case 's':
            dir = {0, 1};
            break;
        case 'D':
        case 'd':
            dir = {1, 0};
            break;
        default:
            return;
    }

    const Point player_pos = point_add(state.player, dir);

    // check the bounds
    if (player_pos.x < 0 || player_pos.x >= state.columns) return;
    if (player_pos.y < 0 || player_pos.y >= state.rows) return;

    // check if the player is going to run into the wall
    if (state_is_solid(state, player_pos)) return;

    // handle blocks
    if (state_is_block(state, player_pos)) {
        const Point block_pos = point_add(player_pos, dir);

        // check bounds for the block
        if (block_pos.x < 0 || block_pos.x >= state.columns) return;
        if (block_pos.y < 0 || block_pos.y >= state.rows) return;

        // make sure spot is not a block or a solid
        if (state_is_block(state, block_pos) || state_is_solid(state, block_pos)) return;

        // update block position
        state_set_block(state, block_pos);
        state_clear_block(state, player_pos);
        state.player = player_pos;
    } else {
        state.player = player_pos;
    }
}

void state_render(const State& state) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    move(2, (max_x - 7) / 2);
    printw("Sokoban");

    const int start_x = (max_x - state.columns) / 2;
    const int start_y = (max_y - state.rows) / 2;

    Point p;
    for (p.y = 0; p.y < state.rows; ++p.y) {
        const bool player_y = state.player.y == p.y;

        for(p.x = 0; p.x < state.columns; ++p.x) {
            move(start_y + p.y, start_x + p.x);

            if (player_y && state.player.x == p.x) {
                addch('@');
            } else if (state_is_solid(state, p)) {
                addch('X');
            } else {
                const bool is_block = state_is_block(state, p);
                const bool is_switch = state_is_switch(state, p);

                if (is_block && is_switch) {
                    addch('B');
                } else if (is_block) {
                    addch('b');
                } else if(is_switch) {
                    addch('.');
                }
            }

        }
    }
}

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

    State state;
    state_init_from_level(state, level);

    initscr();
    curs_set(0); // invisible cursor

    // Game loop
    state_render(state);
    while (!state_game_over(state)) {
        state_update(state);
        clear();
        state_render(state);
    }

    move(0, 0);
    printw("You won!");
    getch(); // pause before ending
    endwin();


    free(state.blocks);
    free(state.solids);
    free(state.switches);

    return 0;
}