
#pragma once

#include "state.hpp"
#include <cstddef>

struct LevelProgression : State {
    State* menu = nullptr;
    State* game = nullptr;

    std::vector<std::string> levels;
    int render_start_row = 0;

    LevelProgression(ApplicationState* state) : State(state) {
        _load_progression();
    }

    void _load_progression();

    void on_enter() override;
    void on_exit() override;

    State* update() override;
    void render() const override;
};