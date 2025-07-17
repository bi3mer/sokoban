#include "sokoban.hpp"
#include "command.hpp"
#include "constants.hpp"
#include "point.hpp"
#include <cstring>
#include <iostream>

void sokoban_init_from_level(Sokoban& state, const std::vector<std::string>& level) {
    memset(state.original_blocks, 0, LEVEL_MAX_WIDTH * LEVEL_MAX_HEIGHT / 8);
    memset(state.blocks, 0, LEVEL_MAX_WIDTH * LEVEL_MAX_HEIGHT / 8);
    memset(state.solids, 0, LEVEL_MAX_WIDTH * LEVEL_MAX_HEIGHT / 8);
    memset(state.switches, 0, LEVEL_MAX_WIDTH * LEVEL_MAX_HEIGHT / 8);

    state.rows = level.size();
    state.columns = level[0].size();

    Point p;
    for (p.y = 0; p.y < state.rows; ++p.y) {
        const std::string& row = level[p.y];

        for (p.x = 0; p.x < state.columns; ++p.x) {
            switch (row[p.x]) {
                case ' ':
                    break;
                case 'X':
                    sokoban_set_solid(state, p);
                    break;
                case 'b':
                    sokoban_set_block(state, p);
                    break;
                case '.':
                    sokoban_set_switches(state, p);
                    break;
                case 'B':
                    sokoban_set_block(state, p);
                    sokoban_set_switches(state, p);
                    break;
                case '@':
                    state.player = p;
                    state.original_player = p;
                    break;
                default:
                    std::cerr << "Unnknown tile type: " << row[p.x] << std::endl;
                    assert(false);
            }
        }
    }
}

bool sokoban_game_over(const Sokoban& state) {
    const std::size_t num_bytes = (state.rows * state.columns + 7) / 8;
    return !memcmp(state.switches, state.blocks, num_bytes);
}

void sokoban_restart(Sokoban& state) {
    const std::size_t num_bytes = (state.rows * state.columns + 7) / 8;
    memcpy(state.blocks, state.original_blocks, num_bytes);

    state.player = state.original_player;
}

Command sokoban_update(Sokoban& state, const Point& direction) {
    const Point player_pos = point_add(state.player, direction);
    Command command = {
        .moved_block = false,
        .player_direction = { 0, 0 }
    };

    // check the bounds
    if (player_pos.x < 0 || player_pos.x >= state.columns) return command;
    if (player_pos.y < 0 || player_pos.y >= state.rows) return command;

    // check if the player is going to run into the wall
    if (sokoban_is_solid(state, player_pos)) return command;

    // handle blocks
    if (sokoban_is_block(state, player_pos)) {
        const Point block_pos = point_add(player_pos, direction);

        // check bounds for the block
        if (block_pos.x < 0 || block_pos.x >= state.columns) return command;
        if (block_pos.y < 0 || block_pos.y >= state.rows) return command;

        // make sure spot is not a block or a solid
        if (sokoban_is_block(state, block_pos) || sokoban_is_solid(state, block_pos)) return command;

        // update block position
        sokoban_set_block(state, block_pos);
        sokoban_clear_block(state, player_pos);
        state.player = player_pos;

        command.moved_block = true;
        command.player_direction = direction;
    } else {
        state.player = player_pos;
        command.player_direction = direction;
    }

    return command;
}

void sokoban_undo(Sokoban& state, const Command& command) {
    const Point undo_direction = {
        .x = -command.player_direction.x,
        .y = -command.player_direction.y
    };

    const Point original_player_position = state.player;
    sokoban_update(state, undo_direction);

    if (command.moved_block) {
        sokoban_set_block(state, original_player_position);
        sokoban_clear_block(state, point_add(original_player_position, command.player_direction));
    }
}