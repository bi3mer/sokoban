#pragma once

#include "sokoban.hpp"


#define UI_HIGHLIGHTED 0
#define UI_REGULAR 1

void ui_init();
void ui_close();
char ui_get_char();

void ui_render_sokoban(const Sokoban&);