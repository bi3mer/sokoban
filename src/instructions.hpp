#pragma once
#include "application_state.hpp"
#include "state.hpp"

struct Instructions : State {
    State* menu;

    Instructions(ApplicationState& state) : State(state) {};

    State* update() override;
    void render() const override;
};