
#pragma once
#include "game.hpp"
#include "state.hpp"
#include <string>
#include <vector>

struct LevelProgression : State {
    State* menu;
    Game* game;

    std::size_t selected_index = 0;
    std::vector<std::string> levels;

    LevelProgression();
    void _load_progression();
    State* update() override;
    void render() const override;
};