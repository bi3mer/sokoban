
#pragma once

#include "state.hpp"
#include <cstddef>

struct LevelProgression : State {
    std::vector<std::string> levels;
    State* menu = nullptr;
    State* game = nullptr;

    LevelProgression(ApplicationState* state) : State(state) {
        _load_progression();
    }

    void _load_progression();
    State* update() override;
    void render() const override;
};