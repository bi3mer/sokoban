#pragma once

#include <cstddef>
#include <vector>

struct LevelData {
    std::size_t moves;
    double seconds;
};

struct ApplicationState {
    std::vector<LevelData> level_data;
    int level_id = 0;

    ApplicationState();

    void unlock_new_level();
    void update(const LevelData&);
    void save() const;
};