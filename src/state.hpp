#pragma once

#include "application_state.hpp"

struct State {
    ApplicationState* app_state;

    State(ApplicationState* state) : app_state(state) {};

    virtual State* update() = 0;
    virtual void render() const = 0;
};