#include "sokoban.hpp"
#include "command.hpp"
#include "constants.hpp"
#include "log.hpp"
#include "point.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iterator>

bool sokoban_init_from_level(Sokoban& state, char const * const level_path) {
    Log::info(
        std::format("sokoban :: loading level: {}",level_path).c_str()
    );

    state.columns = 0;

    state.original_blocks.reset();
    state.blocks.reset();
    state.solids.reset();
    state.switches.reset();

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
            state.solids.set(sokoban_index(state, p));
            ++p.x;
            break;
        case 'b':
            state.blocks.set(sokoban_index(state, p));
            ++p.x;
            break;
        case '.':
            state.switches.set(sokoban_index(state, p));
            ++p.x;
            break;
        case 'B':
            state.blocks.set(sokoban_index(state, p));
            state.switches.set(sokoban_index(state, p));
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

    state.original_blocks = state.blocks;
    state.rows = static_cast<std::size_t>(p.y) + 1;

    return false;
}

bool sokoban_game_over(const Sokoban& state) {
    return state.switches == state.blocks;
}

void sokoban_restart(Sokoban& state) {
    state.blocks = state.original_blocks;
    state.player = state.original_player;
}

Command sokoban_update(Sokoban& state, const Point& direction) {
    const Point player_pos = point_add(state.player, direction);
    const std::size_t player_pos_index = sokoban_index(state, player_pos);
    Command command = {
        .moved_block = false,
        .player_direction = { 0, 0 }
    };

    // check the bounds
    if (player_pos.x < 0 || player_pos.x >= state.columns) return command;
    if (player_pos.y < 0 || player_pos.y >= state.rows) return command;

    // check if the player is going to run into the wall
    if (state.solids[player_pos_index]) return command;

    // handle blocks
    if (state.blocks[player_pos_index]) {
        const Point block_pos = point_add(player_pos, direction);
        const std::size_t block_pos_index = sokoban_index(state, block_pos);

        // check bounds for the block
        if (block_pos.x < 0 || block_pos.x >= state.columns) return command;
        if (block_pos.y < 0 || block_pos.y >= state.rows) return command;

        // make sure spot is not a block or a solid
        if (state.blocks[block_pos_index] || state.solids[block_pos_index])
            return command;

        // update block position
        state.blocks.set(block_pos_index);
        state.blocks.reset(player_pos_index);
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
        state.blocks.set(sokoban_index(state, original_player_position));
        state.blocks.reset(sokoban_index(state, point_add(
            original_player_position,
            command.player_direction
        )));
    }
}