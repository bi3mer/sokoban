#pragma once

#include "ring_buffer.hpp"
#include "PooledLinkedList.hpp"
#include "application_state.hpp"
#include "command.hpp"
#include "constants.hpp"
#include "state.hpp"

struct Game : State {
    State* level_progression;
    // PooledLinkedList<Command> commands;
    RingBuffer<Command, MAX_COMMANDS> commands;

    Game(ApplicationState* state) : State(state), commands(MAX_COMMANDS) {};

    void on_enter() override;
    void on_exit() override;

    State* update() override;
    void render() const override;
};