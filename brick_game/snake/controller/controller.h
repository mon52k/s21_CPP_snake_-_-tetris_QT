#ifndef CPP3_BRICKGAME_SRC_BRICK_GAME_SNAKE_CONTROLLER_CONTROLLER_H_
#define CPP3_BRICKGAME_SRC_BRICK_GAME_SNAKE_CONTROLLER_CONTROLLER_H_
#include "../model/snake.h"

namespace s21 {

/**
 * @brief Класс Контроллера. Отвечает за взаимодействие Модели и Консоли
 * @ingroup SnakeGame
 */
class Controller {
 public:
  Snake *game;  ///< Сыллка на объект класса Snake

  Controller(Snake *game);
  ~Controller() noexcept;
  void userInput(UserAction_t action, bool hold);
  GameInfo_t updateCurrentState();
};

}  // namespace s21
#endif  // CPP3_BRICKGAME_SRC_BRICK_GAME_SNAKE_CONTROLLER_CONTROLLER_H_
