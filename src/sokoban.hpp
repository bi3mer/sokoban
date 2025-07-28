#pragma once

#include <bitset>
#include <cstddef>
#include <assert.h>

#include "command.hpp"
#include "constants.hpp"
#include "point.hpp"

struct Sokoban {
    std::size_t rows = 0;
    std::size_t columns = 0;
    Point player = { 0, 0 };
    Point original_player;

    std::bitset<LEVEL_LENGTH> original_blocks;
    std::bitset<LEVEL_LENGTH> blocks;
    std::bitset<LEVEL_LENGTH> solids;
    std::bitset<LEVEL_LENGTH> switches;
};

inline std::size_t sokoban_index(const Sokoban& state, Point p) {
    return static_cast<std::size_t>(p.y) * state.columns + static_cast<std::size_t>(p.x);
}

bool sokoban_init_from_level(Sokoban&, char const * const);
bool sokoban_game_over(const Sokoban&);
void sokoban_restart(Sokoban&);
Command sokoban_update(Sokoban&, const Point&);
void sokoban_undo(Sokoban&, const Command&);