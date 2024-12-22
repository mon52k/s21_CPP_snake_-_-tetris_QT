#ifndef CPP3_BRICKGAME_SRC_BRICK_GAME_SNAKE_MODEL_SNAKE_H_
#define CPP3_BRICKGAME_SRC_BRICK_GAME_SNAKE_MODEL_SNAKE_H_

#define WIDTH 10
#define HEIGHT 20
#define NOT_STARTED 0
#define STARTED 1
#define PAUSED 2
#define LOSED 3
#define QUIT 4
#define WIN 5
#define timet 30
#define SCORE_FILE_SNAKE "SnakeHighScore.txt"

#include <time.h>

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace s21 {

/**
 * @defgroup SnakeGame Snake Game
 * Классы и структуры использующиеся в игре Snake
 */

/*!
 * @brief Структура - Перечисление возможных действий пользователя в игре.
 * @ingroup SnakeGame
 */
typedef enum {
  Start,      ///< Начало игры.
  Pause,      ///< Пауза игры.
  Terminate,  ///< Завершение игры.
  Left,       ///< Перемещение фигуры влево.
  Right,      ///< Перемещение фигуры вправо.
  Up,         ///< Вращение фигуры.
  Down,       ///< Перемещение фигуры вниз.
  Action      ///< Вращение фигуры.
} UserAction_t;

/**
 * @brief Структура, содержащая информацию о текущем состоянии игры.
 *  * @ingroup SnakeGame
 */
typedef struct {
  int **field;  ///< Игровое поле (двумерный массив).
  int **next;   ///< хранит координаты яблока
  int score;    ///< Текущий счёт игрока.
  int high_score;  ///< Максимальный счёт.
  int level;       ///< Уровень игры.
  int speed;       ///< Скорость игры.
  int pause;  ///< Статус паузы (Принимает значение от 0 до 4).
} GameInfo_t;

/**
 * @brief Класс, отвечающий за управление игрой
 * @ingroup SnakeGame
 */
class Snake {
 public:
  /**
   * @brief Структура, содержащая информацию о координатах элемента змейки.
   *  * @ingroup SnakeGame
   */
  typedef struct {
    int x;  ///< координата x
    int y;  ///< координата y
  } SnakeElement;

  GameInfo_t gameInfo;  ///< Информация о текущем состоянии игры.
  std::vector<SnakeElement> snakeCoordinates;  ///< Координаты змейки.
  struct timespec last_move_time;  ///< время последнего обновления игры
  bool flagMoved;  ///< флаг, указывающий, что змейка переместилась

  Snake();
  ~Snake() noexcept;
  bool CreateField(int ***field, int m, int n);
  void InitialField(int ***field, int m, int n);

  void MoveDown() noexcept;
  void MoveLeft() noexcept;
  void MoveRight() noexcept;
  void MoveUp() noexcept;

  void CheckEndGame() noexcept;
  void UpdateLevel() noexcept;

  void ReadHighScore() noexcept;
  void WriteHighScore();

  void GamePaused() noexcept;
  void GameTerminated() noexcept;
  void GameStart() noexcept;

  bool AteSelf() noexcept;

  void MovingSnake() noexcept;

  UserAction_t GetDirection() noexcept;
  void SetDirection(UserAction_t direction) noexcept;

  void GenerateApple() noexcept;
  bool CheckEatApple() noexcept;
  bool IsSnakeBody(int x, int y) noexcept;
  void InitialSnake();

  bool GetFlagErrorGame() noexcept;

 private:
  UserAction_t direction_;  ///< Направление движения змейки
  bool flagError_;          ///< Флаг успешной работы игры
};

}  // namespace s21

#endif  // CPP3_BRICKGAME_SRC_BRICK_GAME_SNAKE_MODEL_SNAKE_H_
