#include "snake.h"

namespace s21 {

/**
 * @brief Конструктор Snake.
 * Инициализирует начальные данные
 */
Snake::Snake() {
  flagError_ = false;

  gameInfo.pause = NOT_STARTED;
  gameInfo.level = 1;
  gameInfo.speed = 600;
  gameInfo.score = 0;

  ReadHighScore();

  InitialSnake();

  SetDirection(Up);

  clock_gettime(CLOCK_REALTIME, &last_move_time);
  flagMoved = true;
  flagError_ = CreateField(&gameInfo.field, HEIGHT, WIDTH);
  if (!flagError_) {
    flagError_ = CreateField(&gameInfo.next, 1, 2);
    if (!flagError_) GenerateApple();
  }
}

/**
 * @brief Деструктор Snake.
 * Освобождает память, выделенную для игрового поля
 */
Snake::~Snake() noexcept {
  for (int i = 0; i < HEIGHT; i++) delete[] gameInfo.field[i];
  delete[] gameInfo.field;
  for (int i = 0; i < 1; i++) delete[] gameInfo.next[i];
  delete[] gameInfo.next;
}

/**
 * @brief Создает игровое поле.
 *
 * @param field Указатель на поле.
 * @param m Количество строк.
 * @param n Количество столбцов.
 * @return bool Возвращает true, если поле успешно создано, или false при
 * ошибке.
 */
bool Snake::CreateField(int ***field, int m, int n) {
  *field = new int *[m];  //
  if (*field == NULL) {
    flagError_ = true;
  } else {
    for (int i = 0; i < m && !flagError_; i++) {
      (*field)[i] = new int[n];
      if ((*field)[i] == NULL) {
        for (int j = 0; j < i; j++) {
          delete[] (*field)[j];
        }
        flagError_ = true;
      }
    }
  }
  if (!flagError_) InitialField(field, m, n);
  return flagError_;
}

/**
 * @brief Инициализирует игровое поле нулями.
 *
 * @param field Указатель на поле.
 * @param m Количество строк.
 * @param n Количество столбцов.
 */
void Snake::InitialField(int ***field, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      (*field)[i][j] = 13 - (i + j) % 2;
    }
  }
}

/**
 * @brief Инициализация начальных координат змейки
 */
void Snake::InitialSnake() {
  if (!flagError_)
    snakeCoordinates = {{WIDTH / 2, HEIGHT / 2 - 1},
                        {WIDTH / 2, HEIGHT / 2},
                        {WIDTH / 2, HEIGHT / 2 + 1},
                        {WIDTH / 2, HEIGHT / 2 + 2}};
}

/**
 * @brief Приостанавливает или возобновляет игру.
 */
void Snake::GamePaused() noexcept {
  if (gameInfo.pause == PAUSED || gameInfo.pause == STARTED)
    gameInfo.pause = gameInfo.pause == 1 ? PAUSED : STARTED;
}

/**
 * @brief Завершает игру, сохраняя рекорд.
 *
 */
void Snake::GameTerminated() noexcept {
  WriteHighScore();
  gameInfo.pause = QUIT;
}

/**
 * @brief Запускает игру, если она не начата.
 *
 */
void Snake::GameStart() noexcept {
  if (gameInfo.pause == NOT_STARTED) {
    gameInfo.pause = STARTED;
  }
}

/**
 * @brief Считывает рекордный счет из файла.
 *
 */
void Snake::ReadHighScore() noexcept {
  int high_score = 0;
  FILE *f = fopen(SCORE_FILE_SNAKE, "r");
  if (f != NULL) {
    int res = fscanf(f, "%d", &high_score);
    if (!res) high_score = 0;
    fclose(f);
  }
  gameInfo.high_score = high_score;
}

/**
 * @brief Записывает рекордный счет в файл.
 *
 * @param game Указатель на структуру Tetris.
 */
void Snake::WriteHighScore() {
  if (gameInfo.score >= gameInfo.high_score) {
    gameInfo.high_score = gameInfo.score;
    FILE *f = fopen(SCORE_FILE_SNAKE, "w");
    if (f != NULL) {
      fprintf(f, "%d", gameInfo.high_score);
      fclose(f);
    }
  }
}

/**
 * @brief Получение направления змейки
 * @return UserAction_t Возвращает направление змейки.
 */
UserAction_t Snake::GetDirection() noexcept { return this->direction_; }

/**
 * @brief Изменение текущего направления змейки
 *
 */
void Snake::SetDirection(UserAction_t direction) noexcept {
  this->direction_ = direction;
}

/**
 * @brief Изменение направления змейки в позицию Down
 *
 * Изменяется направление, если змейка не находится в направлении Up
 *
 */
void Snake::MoveDown() noexcept {
  if (this->direction_ != Up) {
    this->direction_ = Down;
    flagMoved = false;
  }
}

/**
 * @brief Изменение направления змейки в позицию Up
 *
 * Изменяется направление, если змейка не находится в направлении Down
 *
 */
void Snake::MoveUp() noexcept {
  if (this->direction_ != Down) {
    this->direction_ = Up;
    flagMoved = false;
  }
}

/**
 * @brief Изменение направления змейки в позицию Left
 *
 * Изменяется направление, если змейка не находится в направлении Right
 *
 */
void Snake::MoveLeft() noexcept {
  if (this->direction_ != Right) {
    this->direction_ = Left;
    flagMoved = false;
  }
}

/**
 * @brief Изменение направления змейки в позицию Roght
 *
 * Изменяется направление, если змейка не находится в направлении Left
 *
 */
void Snake::MoveRight() noexcept {
  if (this->direction_ != Left) {
    this->direction_ = Right;
    flagMoved = false;
  }
}

/**
 * @brief Функция движения Змейки.
 *
 * В зависимости от направления добавляет новую кординаты  к змейке
 *
 */
void Snake::MovingSnake() noexcept {
  CheckEndGame();
  if (gameInfo.pause != STARTED) {
    return;
  }

  int dx, dy;
  switch (GetDirection()) {
    case Up:
      dx = 0;
      dy = -1;
      break;
    case Down:
      dx = 0;
      dy = 1;
      break;
    case Left:
      dx = -1;
      dy = 0;
      break;
    case Right:
      dx = 1;
      dy = 0;
      break;
    default:
      dx = 0;
      dy = 0;
  }
  SnakeElement newCoord{snakeCoordinates.front().x + dx,
                        snakeCoordinates.front().y + dy};
  auto iter = snakeCoordinates.cbegin();
  snakeCoordinates.insert(iter, newCoord);
  if (CheckEatApple()) {
    GenerateApple();
    WriteHighScore();
  } else {
    snakeCoordinates.pop_back();
  }
  flagMoved = true;
}

/**
 * @brief Проверка на съедение яблока
 *
 */
bool Snake::CheckEatApple() noexcept {
  bool flag = false;
  if (snakeCoordinates.front().x == gameInfo.next[0][0] &&
      snakeCoordinates.front().y == gameInfo.next[0][1]) {
    flag = true;
    gameInfo.score++;
    UpdateLevel();
    if (gameInfo.score > gameInfo.high_score) {
      gameInfo.high_score = gameInfo.score;
    }
  }

  return flag;
}

/**
 * @brief Проверка на столкновение змейки с собой
 *
 */
bool Snake::AteSelf() noexcept {
  bool flag = false;
  for (size_t i = 1; i < snakeCoordinates.size() && !flag; ++i) {
    if (snakeCoordinates[i].x == snakeCoordinates[0].x &&
        snakeCoordinates[i].y == snakeCoordinates[0].y) {
      flag = true;
    }
  }
  return flag;
}

/**
 * @brief Проверка на завершение игры
 *
 */
void Snake::CheckEndGame() noexcept {
  for (int i = 0; i < (int)snakeCoordinates.size(); i++) {
    if (snakeCoordinates.front().x <= 0 && GetDirection() == Left) {
      gameInfo.pause = LOSED;
    } else if (snakeCoordinates.front().x >= WIDTH - 1 &&
               GetDirection() == Right) {
      gameInfo.pause = LOSED;
    } else if (snakeCoordinates.front().y <= 0 && GetDirection() == Up) {
      gameInfo.pause = LOSED;
    } else if (snakeCoordinates.front().y >= HEIGHT - 1 &&
               GetDirection() == Down) {
      gameInfo.pause = LOSED;
    }
  }
  if (AteSelf()) {
    gameInfo.pause = LOSED;
  }
  if (gameInfo.score == 196) {
    gameInfo.pause = WIN;
  }
}

/**
 * @brief Проверка занимает ли клетка данную координату
 *
 */
bool Snake::IsSnakeBody(int x, int y) noexcept {
  for (const auto &segment : snakeCoordinates) {
    if (segment.x == x && segment.y == y) {
      return true;
    }
  }
  return false;
}

/**
 * @brief Функция генерации нового яблока
 *
 */
void Snake::GenerateApple() noexcept {
  std::vector<SnakeElement> availablePositions;

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (!IsSnakeBody(x, y)) {
        availablePositions.push_back({x, y});
      }
    }
  }

  if (!availablePositions.empty()) {
    int random_index = rand() % availablePositions.size();
    gameInfo.next[0][0] = availablePositions[random_index].x;
    gameInfo.next[0][1] = availablePositions[random_index].y;
  }
}

/**
 * @brief Функция повышения уровня
 *
 * Макс уровень - 10. Повышается каждые 5 очков (до 50)
 *
 */
void Snake::UpdateLevel() noexcept {
  gameInfo.level = (gameInfo.score / 50) >= 1 ? 10 : 1 + gameInfo.score / 5;
  gameInfo.speed = 600 - gameInfo.level * 40;
}

/**
 * @brief Получение Флага игры ошибки
 *
 * return false - игра начинается, Создание динамических массивов прошла успешна
 * return true - игра не начнется. Ошибка при создании динамических массивов
 *
 */
bool Snake::GetFlagErrorGame() noexcept { return flagError_; }

}  // namespace s21
