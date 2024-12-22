#ifndef CPP3_BRICKGAME_SRC_GUI_CLI_MAINCONSOLE_H_
#define CPP3_BRICKGAME_SRC_GUI_CLI_MAINCONSOLE_H_

#include "snake/snake_console.h"
#include "tetris/tetris_frontend.h"

namespace s21 {
bool startChoosedGame(int *choosedPoint);
bool handleInputMenu(int *choosedPoint);
void drawMenu(int choosedPoint);
}  // namespace s21

int main();
#endif  // CPP3_BRICKGAME_SRC_GUI_CLI_MAINCONSOLE_H_