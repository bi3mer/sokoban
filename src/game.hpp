#pragma once

#include "PooledLinkedList.hpp"
#include "application_state.hpp"
#include "point.hpp"
#include "state.hpp"

struct Game : State {
    State* level_progression;
    PooledLinkedList<Point> commands;

    Game(ApplicationState* state) : State(state) {};
    ~Game();
    State* update() override;
    void render() const override;
};