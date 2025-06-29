#pragma once
#include "state.hpp"

struct Instructions : State {
    State* menu;

    State* update() override;
    void render() const override;
};