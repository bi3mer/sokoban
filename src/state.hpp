#pragma once

#include "application_state.hpp"
struct State {
    ApplicationState& application_state;

    State(ApplicationState& _state) :  application_state(_state) {}

    virtual State* update() = 0;
    virtual void render() const = 0;
};