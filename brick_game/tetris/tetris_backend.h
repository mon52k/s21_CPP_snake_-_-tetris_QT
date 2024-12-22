#ifndef CPP3_BRICKGAME_SRC_BRICK_GAME_TETRIS_TETRIS_BACKEND_H_
#define CPP3_BRICKGAME_SRC_BRICK_GAME_TETRIS_TETRIS_BACKEND_H_
#define WIDTH 10
#define HEIGHT 20
#define OK_ 0
#define ERROR 2
#define NOT_STARTED 0
#define STARTED 1
#define PAUSED 2
#define ENDED 3
#define QUIT 4
#define timet 30
#define SCORE_FILE "TetrisHighScore.txt"

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/**
 * @defgroup TetrisGame Tetris Game
 * Структуры, использующиеся в игре Tetris
 * @{
 */
/**
 * @brief Перечисление возможных действий пользователя в игре.
 * @ingroup TetrisGame
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
 * @ingroup TetrisGame
 */
typedef struct {
  int **field;  ///< Игровое поле (двумерный массив).
  int **next;  ///< Следующая фигура (двумерный массив).
  int score;       ///< Текущий счёт игрока.
  int high_score;  ///< Максимальный счёт.
  int level;       ///< Уровень игры.
  int speed;       ///< Скорость игры.
  int pause;  ///< Статус паузы (Принимает значение от 0 до 4).
} GameInfo_t;

/**
 * @brief Структура, описывающая текущую фигуру на игровом поле.
 * @ingroup TetrisGame
 */
typedef struct {
  int x;        ///< Позиция фигуры по оси X.
  int y;        ///< Позиция фигуры по оси Y.
  int **shape;  ///< Форма фигуры (двумерный массив).
  int indexTetramino;  ///< Индекс текущей фигуры (тетромино).
  int indexNext;  ///< Индекс следующей фигуры.
} Figure_t;

/**
 * @brief Главная структура игры Tetris, содержащая информацию об игре и
 * фигурах.
 * @ingroup TetrisGame
 */
typedef struct {
  GameInfo_t gameInfo;  ///< Информация о текущем состоянии игры.
  Figure_t figure;  ///< Текущая фигура.
  double speed;     ///< Текущая скорость игры.
  bool flag;  ///< Флаг, используемый для управления игровым процессом.
} Tetris;

/**
 * @defgroup TetrisBackend Tetris Backend
 * Группа, описывающая внутреннюю логику игры (бекенд).
 * @ingroup TetrisGame
 * @{
 */
void userInput(Tetris *game, UserAction_t action,
               bool hold);  // overrided from readme

GameInfo_t updateCurrentState(Tetris *game);

int createField(int ***field, int m, int n);
void initialField(int ***field, int m, int n);
void initializeFigure(Tetris *game);
int initialGame(Tetris *game);

void checkLockFigure(Tetris *game);
void lockFigure(Tetris *game);

void moveDown(Tetris *game);
void moveLeft(Tetris *game);
void moveRight(Tetris *game);
void rotate(Tetris *game);
int checkRotate(Tetris *game);

void cpyTetraminoFigure(int ***field, int indexTetramino);

void attachingFigures(Tetris *game);
bool checkLose(Tetris *game);
void changeScore(Tetris *game, int counter);
void updateLevel(Tetris *game);

void readHighScore(Tetris *game);
void writeHighScore(Tetris *game);
bool isValidPosition(Tetris *game, int diffX, int diifY);

void gamePaused(Tetris *game);
void gameTerminated(Tetris *game);
void gameStart(Tetris *game);
void freeSpace(Tetris *game);
#endif  // CPP3_BRICKGAME_SRC_BRICK_GAME_TETRIS_TETRIS_BACKEND_H_

/** @} */  // TetrisBackend

/** @} */  // TetrisGame