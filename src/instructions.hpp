#pragma once
#include "state.hpp"

struct Instructions : State {
    State* menu;

    Instructions(ApplicationState* state) : State(state) {};

    void on_enter() override;
    void on_exit() override;

    State* update() override;
    void render() const override;
};