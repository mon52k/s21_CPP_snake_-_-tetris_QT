#include "controller.h"

namespace s21 {

/**
 * @brief Конструктор класса Controller.
 *
 * @param game Указатель на объект класса Snake.
 */
Controller::Controller(Snake *game) : game(game) {}

/**
 * @brief Функция обработки переданной командой
 *
 * @param action Действие игрока
 * @param game Показатель зажатия клавиши
 *
 */
void Controller::userInput(UserAction_t action, bool hold) {
  if (game->gameInfo.pause == STARTED) {
    if (game->flagMoved) {
      switch (action) {
        case Left:
          game->MoveLeft();
          break;
        case Right:
          game->MoveRight();
          break;
        case Down:
          game->MoveDown();
          break;
        case Up:
          game->MoveUp();
          break;
        case Action:
          if (hold) game->MovingSnake();
          break;
        default:
          break;
      }
    }
  }

  switch (action) {
    case Terminate:
      game->GameTerminated();
      break;
    case Pause:
      game->GamePaused();
      break;
    case Start:
      game->GameStart();
      break;
    default:
      break;
  }
}

/**
 * @brief Функция обновления игрового состояния
 *
 */
GameInfo_t Controller::updateCurrentState() {
  GameInfo_t gm;
  struct timespec current;
  double difference;
  clock_gettime(CLOCK_REALTIME, &current);
  difference = (current.tv_sec - game->last_move_time.tv_sec) * 1000 +
               ((current.tv_nsec - game->last_move_time.tv_nsec)) / 1000000;
  if (difference >= game->gameInfo.speed) {
    game->MovingSnake();
    game->last_move_time = current;
  }

  return gm;
}

/**
 * @brief Деструктор класса Controller.
 *
 */
Controller::~Controller() noexcept {}
}  // namespace s21
