
#pragma once

#include "game.hpp"
#include "state.hpp"
#include <cstddef>
#include <string>
#include <vector>

struct LevelProgression : State {
    std::size_t selected_index = 0;
    std::vector<std::string> levels;
    State* menu = nullptr;
    Game* game = nullptr;

    LevelProgression();
    void _load_progression();
    State* update() override;
    void render() const override;
};