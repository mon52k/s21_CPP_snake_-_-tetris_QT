#ifndef CPP3_BRICKGAME_SRC_GUI_CLI_SNAKE_SNAKE_CONSOLE_H_
#define CPP3_BRICKGAME_SRC_GUI_CLI_SNAKE_SNAKE_CONSOLE_H_

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../../../brick_game/snake/controller/controller.h"

namespace s21 {
void initializeNcurses();
void printRectangle(int top_y, int bottom_y, int left_x, int right_x);

/**
 * @brief Класс, отвечающий за отрисовку игры Змейка в консоли
 * @ingroup SnakeGame
 */
class SnakeConsole {
 public:
  SnakeConsole(Controller *controller);
  ~SnakeConsole() noexcept;

  void start();

 private:
  Controller *controller;  ///< Ссылка на объект класса Controller
  void Draw();
  void InitialGamebar();
  void HandleInput();
  void DrawSnake();
  void DrawApple() noexcept;
};

}  // namespace s21

#endif  // CPP3_BRICKGAME_SRC_GUI_CLI_SNAKE_SNAKE_CONSOLE_H_