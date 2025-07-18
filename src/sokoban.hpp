#pragma once

#include <cstddef>
#include <vector>
#include <assert.h>

#include "command.hpp"
#include "constants.hpp"
#include "point.hpp"
#include "bit_operations.hpp"

struct Sokoban {
    std::size_t rows = 0;
    std::size_t columns = 0;
    Point player = { 0, 0 };
    Point original_player;

    // divide by 8 because there are 8 bits in one unsigned char
    unsigned char original_blocks[LEVEL_MAX_WIDTH * LEVEL_MAX_HEIGHT / 8];
    unsigned char blocks[LEVEL_MAX_WIDTH * LEVEL_MAX_HEIGHT / 8];
    unsigned char solids[LEVEL_MAX_WIDTH * LEVEL_MAX_HEIGHT / 8];
    unsigned char switches[LEVEL_MAX_WIDTH * LEVEL_MAX_HEIGHT / 8];
};

inline std::size_t sokoban_index(const Sokoban& state, Point p) {
    return static_cast<std::size_t>(p.y) * state.columns + static_cast<std::size_t>(p.x);
}

inline void sokoban_set_block(Sokoban& state, Point p) {
    set_uchar_bit(state.blocks, sokoban_index(state, p));
}

inline void sokoban_clear_block(Sokoban& state, Point p) {
    clear_uchar_bit(state.blocks, sokoban_index(state, p));
}

inline bool sokoban_is_block(const Sokoban& state, Point p) {
    return get_uchar_bit((unsigned char*) state.blocks, sokoban_index(state, p));
}

inline void sokoban_set_solid(Sokoban& state, Point p) {
    set_uchar_bit(state.solids, sokoban_index(state, p));
}

inline bool sokoban_is_solid(const Sokoban& state, Point p) {
    return get_uchar_bit((unsigned char*) state.solids, sokoban_index(state, p));
}

inline void sokoban_set_switches(Sokoban& state, Point p) {
    set_uchar_bit(state.switches, sokoban_index(state, p));
}

inline bool sokoban_is_switch(const Sokoban& state, Point p) {
    return get_uchar_bit((unsigned char*) state.switches, sokoban_index(state, p));
}

bool sokoban_init_from_level(Sokoban&, char const * const);
bool sokoban_game_over(const Sokoban&);
void sokoban_restart(Sokoban&);
Command sokoban_update(Sokoban&, const Point&);
void sokoban_undo(Sokoban&, const Command&);