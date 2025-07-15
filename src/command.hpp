#pragma once

#include "point.hpp"

struct Command {
    bool moved_block;
    Point player_direction;
};