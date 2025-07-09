#pragma once

#include "application_state.hpp"

struct State {
    ApplicationState* application_state;

    State(ApplicationState* state) : application_state(state) {};

    virtual State* update() = 0;
    virtual void render() const = 0;
};