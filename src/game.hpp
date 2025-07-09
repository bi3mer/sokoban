#pragma once

#include "application_state.hpp"
#include "state.hpp"

struct Game : State {
    State* level_progression;

    Game(ApplicationState* state) : State(state) {};
    ~Game();
    State* update() override;
    void render() const override;
};