#pragma once

#include "sokoban.hpp"
void ui_init();
void ui_close();
char ui_get_char();

void ui_render_sokoban(const Sokoban&);