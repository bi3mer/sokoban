#pragma once

#include "point.hpp"

struct Command {
    bool moved_block;
    Point player_direction;

    bool equals(const Command& other) const noexcept {
        return other.moved_block == moved_block &&
               player_direction.equals(other.player_direction);
    }
};