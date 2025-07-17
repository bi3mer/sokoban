#pragma once

#include <cstddef>
const int COLOR_HIGHLIGHTED = 1;
const int COLOR_REGULAR = 2;
const int COLOR_UNLOCKED = 3;
const int COLOR_UNLOCKED_HIGHLIGHTED = 4;
const int COLOR_LOCKED = 5;
const int COLOR_LOCKED_HIGHLIGHTED = 6;

const int KEY_ESC = 27;

const char * const LEVELS_DIR = "levels";
const char * const SAVE_FILE = "save.txt";

const std::size_t INITIAL_POOL_SIZE = 50;

char const * const MOVE_MESSAGE = "You beat your move record from %i to %i";
char const * const TIME_MESSAGE = "You beat your time record from %.2f to %.2f";