#pragma once

#include "state.hpp"
#include <cstddef>

struct Menu {
    int user_selection = 0;

    State update(const int);
    void render();
};