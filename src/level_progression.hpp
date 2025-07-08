
#pragma once

#include "application_state.hpp"
#include "game.hpp"
#include "state.hpp"
#include <cstddef>
#include <string>
#include <vector>

struct Game;
struct LevelProgression : State {
    std::vector<std::string> levels;
    State* menu = nullptr;
    Game* game = nullptr;

    LevelProgression(ApplicationState&);
    void _load_progression();
    State* update() override;
    void render() const override;
};