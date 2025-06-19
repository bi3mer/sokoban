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

    unsigned char* blocks;
    unsigned char* solids;
    unsigned char* switches;
};

inline std::size_t state_index(const Sokoban& state, Point p) {
    return p.y * state.columns + p.x;
}

inline void state_set_block(Sokoban& state, Point p) {
    set_uchar_bit(state.blocks, state_index(state, p));
}

inline void state_clear_block(Sokoban& state, Point p) {
    clear_uchar_bit(state.blocks, state_index(state, p));
}

inline bool state_is_block(const Sokoban& state, Point p) {
    return get_uchar_bit(state.blocks, state_index(state, p));
}

inline void state_set_solid(Sokoban& state, Point p) {
    set_uchar_bit(state.solids, state_index(state, p));
}

inline bool state_is_solid(const Sokoban& state, Point p) {
    return get_uchar_bit(state.solids, state_index(state, p));
}

inline void state_set_switches(Sokoban& state, Point p) {
    set_uchar_bit(state.switches, state_index(state, p));
}

inline bool state_is_switch(const Sokoban& state, Point p) {
    return get_uchar_bit(state.switches, state_index(state, p));
}

void state_init_from_level(Sokoban&, const std::vector<std::string>&);
bool state_game_over(const Sokoban&);
void state_update(Sokoban&, const char);
