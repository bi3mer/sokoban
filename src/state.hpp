#pragma once

struct State {
    virtual State* update() = 0;
    virtual void render() const = 0;
};