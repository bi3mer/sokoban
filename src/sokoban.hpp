#pragma once

#include <cstddef>
#include <vector>
#include <assert.h>

#include "point.hpp"
#include "bit_operations.hpp"


struct Sokoban {
    std::size_t rows = 0;
    std::size_t columns = 0;
    Point player = { 0, 0 };
    Point original_player;

    unsigned char* original_blocks;
    unsigned char* blocks;
    unsigned char* solids;
    unsigned char* switches;
};

inline std::size_t sokoban_index(const Sokoban& state, Point p) {
    return p.y * state.columns + p.x;
}

inline void sokoban_set_block(Sokoban& state, Point p) {
    set_uchar_bit(state.blocks, sokoban_index(state, p));
}

inline void sokoban_clear_block(Sokoban& state, Point p) {
    clear_uchar_bit(state.blocks, sokoban_index(state, p));
}

inline bool sokoban_is_block(const Sokoban& state, Point p) {
    return get_uchar_bit(state.blocks, sokoban_index(state, p));
}

inline void sokoban_set_solid(Sokoban& state, Point p) {
    set_uchar_bit(state.solids, sokoban_index(state, p));
}

inline bool sokoban_is_solid(const Sokoban& state, Point p) {
    return get_uchar_bit(state.solids, sokoban_index(state, p));
}

inline void sokoban_set_switches(Sokoban& state, Point p) {
    set_uchar_bit(state.switches, sokoban_index(state, p));
}

inline bool sokoban_is_switch(const Sokoban& state, Point p) {
    return get_uchar_bit(state.switches, sokoban_index(state, p));
}

void sokoban_init_from_level(Sokoban&, const std::vector<std::string>&);
bool sokoban_game_over(const Sokoban&);
void sokoban_restart(Sokoban&);
void sokoban_update(Sokoban&, const Point&);
void sokoban_free(Sokoban&);