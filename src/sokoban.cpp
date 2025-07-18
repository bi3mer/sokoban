#include "sokoban.hpp"
#include "command.hpp"
#include "constants.hpp"
#include "log.hpp"
#include "point.hpp"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>

bool sokoban_init_from_level(Sokoban& state, char const * const level_path) {
    Log::info(
        std::format("sokoban :: loading level: {}",level_path).c_str()
    );

    state.columns = 0;

    memset(state.original_blocks, 0, LEVEL_MAX_WIDTH * LEVEL_MAX_HEIGHT / 8);
    memset(state.blocks, 0, LEVEL_MAX_WIDTH * LEVEL_MAX_HEIGHT / 8);
    memset(state.solids, 0, LEVEL_MAX_WIDTH * LEVEL_MAX_HEIGHT / 8);
    memset(state.switches, 0, LEVEL_MAX_WIDTH * LEVEL_MAX_HEIGHT / 8);

    std::ifstream file(level_path, std::ios::binary);
    assert(file.is_open());

    Point p = {0, 0};
    char byte;
    while (file.get(byte)) {
        switch (byte) {
        case ' ':
            ++p.x;
            break;
        case 'X':
            sokoban_set_solid(state, p);
            ++p.x;
            break;
        case 'b':
            sokoban_set_block(state, p);
            ++p.x;
            break;
        case '.':
            sokoban_set_switches(state, p);
            ++p.x;
            break;
        case 'B':
            sokoban_set_block(state, p);
            sokoban_set_switches(state, p);
            ++p.x;
            break;
        case '@':
            state.player = p;
            state.original_player = p;
            ++p.x;
            break;
        case '\n':
            ++p.y;
            p.x = 0;
            break;
        default:
            Log::err(std::format("Unnknown tile type: {}", byte).c_str());
            return true;
        }

        if (p.x >= LEVEL_MAX_WIDTH || p.y >= LEVEL_MAX_HEIGHT) {
            Log::err(std::format(
                "{}: width={}, height={} -> limit exceeded.", level_path, p.x, p.y).c_str()
            );

            file.close();
            return true;
        }

        state.columns = std::max(state.columns, static_cast<std::size_t>(p.x));
    }

    file.close();

    state.rows = static_cast<std::size_t>(p.y) + 1;

    return false;
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