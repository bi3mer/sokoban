#pragma once

#include "PooledLinkedList.hpp"
#include "application_state.hpp"
#include "command.hpp"
#include "state.hpp"

struct Game : State {
    State* level_progression;
    PooledLinkedList<Command> commands;

    Game(ApplicationState* state) : State(state) {};
    ~Game();
    State* update() override;
    void render() const override;
};