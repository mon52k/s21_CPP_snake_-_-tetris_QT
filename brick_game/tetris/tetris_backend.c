#include "tetris_backend.h"

/**
 * @brief Создает игровое поле.
 * @param field Указатель на поле.
 * @param m Количество строк.
 * @param n Количество столбцов.
 * @return int Возвращает OK_, если поле успешно создано, или ERROR при ошибке.
 */
int createField(int ***field, int m, int n) {
  int flag = OK_;
  *field = (int **)malloc(sizeof(int *) * m);
  if (*field == NULL) {
    flag = ERROR;
  }
  if (flag == OK_) {
    for (int i = 0; i < m; i++) {
      (*field)[i] = (int *)malloc(sizeof(int) * n);
      if ((*field)[i] == NULL) {
        for (int j = 0; j < i; j++) {
          free((*field)[j]);
          flag = ERROR;
        }
      }
    }
  }
  return flag;
}

/**
 * @brief Инициализирует игровое поле нулями.
 *
 * @param field Указатель на поле.
 * @param m Количество строк.
 * @param n Количество столбцов.
 */
void initialField(int ***field, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      (*field)[i][j] = 0;
    }
  }
}

static bool TETROMINOS[28][4][4] = {
    // Фигуры с поворотом на 0°
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}},  // I
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}},  // L
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}},  // J
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}},  // O
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}},  // S
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}},  // T
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}},  // Z

    // Фигуры с поворотом на 90°
    {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},  // I
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},  // L
    {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}},  // J
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}},  // O
    {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}},  // S
    {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}},  // T
    {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}},  // Z

    // Фигуры с поворотом на 180°
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}},  // I
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}},  // L
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}},  // J
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}},  // O
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}},  // S
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}},  // T
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}},  // Z

    // Фигуры с поворотом на 270°
    {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},  // I
    {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}},  // L
    {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},  // J
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}},  // O
    {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}},  // S
    {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}},  // T
    {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}}   // Z
};

/**
 * @brief Инициализирует фигуру для игры.
 *
 * @param game Указатель на структуру Tetris.
 */
void initializeFigure(Tetris *game) {
  attachingFigures(game);
  game->figure.indexTetramino = game->figure.indexNext;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game->figure.shape[i][j] = game->gameInfo.next[i][j];
    }
  }
  int randomIndex = rand() % 10 * (rand() % 10) % 7;

  cpyTetraminoFigure(&game->gameInfo.next, randomIndex);
  game->figure.indexNext = randomIndex;
  game->figure.x = WIDTH / 2 - 2;
  game->figure.y = -3;
}

/**
 * @brief Инициализирует игру, создавая поле и фигуры, задает начальные
 * параметры.
 *
 * @param game Указатель на структуру Tetris.
 * @return int Возвращает OK_ при успешной инициализации, иначе ERROR.
 */
int initialGame(Tetris *game) {
  game->gameInfo.level = 1;
  game->gameInfo.score = 0;
  game->gameInfo.pause = NOT_STARTED;
  updateLevel(game);
  readHighScore(game);
  game->speed = 1;
  int flag = OK_;
  flag = createField(&(game->gameInfo.field), HEIGHT, WIDTH);
  if (flag == OK_) flag = createField(&(game->gameInfo.next), 4, 4);
  if (flag == OK_) flag = createField(&(game->figure.shape), 4, 4);

  if (flag == OK_) {
    initialField(&game->gameInfo.field, HEIGHT, WIDTH);
    int randomIndex = rand() % 10 * (rand() % 10) % 7;

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        game->gameInfo.next[i][j] = TETROMINOS[randomIndex][i][j];
      }
    }
    game->figure.indexNext = randomIndex;
    initializeFigure(game);
    game->figure.y = -2;
  }

  return flag;
}

/**
 * @brief Обновляет текущее состояние игры, двигая фигуру вниз.
 *
 * @param game Указатель на структуру Tetris.
 * @return GameInfo_t Текущая информация о состоянии игры.
 */
GameInfo_t updateCurrentState(Tetris *game) {
  if (game->gameInfo.pause == STARTED) {
    checkLockFigure(game);
    moveDown(game);
  }
  return game->gameInfo;
}

/**
 * @brief Двигает фигуру вниз на одну позицию.
 *
 * @param game Указатель на структуру Tetris.
 */
void moveDown(Tetris *game) { game->figure.y++; }

/**
 * @brief Проверяет возможность блокировки фигуры в текущей позиции.
 *
 * @param game Указатель на структуру Tetris.
 */
void checkLockFigure(Tetris *game) {
  for (int i = 3; i >= 0; i--) {
    for (int j = 3; j >= 0; j--) {
      int ptr = game->figure.shape[i][j];
      if (game->figure.y + i == HEIGHT - 1) {
        lockFigure(game);

      } else if (ptr == 1 && game->figure.y + i + 1 >= 0 &&
                 (game->gameInfo.field[game->figure.y + i + 1]
                                      [game->figure.x + j] != 0)) {
        if (checkLose(game)) {
          game->gameInfo.pause = ENDED;
        } else {
          lockFigure(game);
        }
      }
    }
  }
}

/**
 * @brief Закрепляет фигуру на игровом поле.
 *
 * @param game Указатель на структуру Tetris.
 */
void lockFigure(Tetris *game) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->figure.shape[i][j] == 1 && game->figure.y + i >= 0 &&
          !game->gameInfo.field[game->figure.y + i][game->figure.x + j]) {
        game->gameInfo.field[game->figure.y + i][game->figure.x + j] =
            game->figure.indexTetramino % 7 + 1;
      }
    }
  }
  initializeFigure(game);
}

/**
 * @brief Проверяет валидность позиции фигуры.
 *
 * @param game Указатель на структуру Tetris.
 * @param diffX Смещение по оси X.
 * @param diffY Смещение по оси Y.
 * @return true Если позиция валидна.
 * @return false Если позиция невалидна.
 */
bool isValidPosition(Tetris *game, int diffX, int diifY) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (game->figure.shape[y][x]) {
        int newX = game->figure.x + x + diffX;
        int newY = game->figure.y + y + diifY;
        if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT ||
            game->gameInfo.field[newY][newX]) {
          return false;
        }
      }
    }
  }
  return true;
}

/**
 * @brief Двигает фигуру влево на одну позицию.
 *
 * @param game Указатель на структуру Tetris.
 */
void moveLeft(Tetris *game) {
  if (isValidPosition(game, -1, 0)) game->figure.x--;
}

/**
 * @brief Двигает фигуру вправо на одну позицию.
 *
 * @param game Указатель на структуру Tetris.
 */
void moveRight(Tetris *game) {
  if (isValidPosition(game, 1, 0)) game->figure.x++;
}

/**
 * @brief Поворачивает фигуру на 90 градусов.
 *
 * @param game Указатель на структуру Tetris.
 */
void rotate(Tetris *game) {
  if (checkRotate(game)) {
    game->figure.indexTetramino = (game->figure.indexTetramino + 7) % 28;
    cpyTetraminoFigure(&game->figure.shape, game->figure.indexTetramino);
  }
}

/**
 * @brief Проверяет возможность поворота фигуры.
 *
 * @param game Указатель на структуру Tetris.
 * @return int Возвращает TRUE, если поворот возможен, иначе FALSE.
 */
int checkRotate(Tetris *game) {
  int **tmp;
  int flag = createField(&tmp, 4, 4);
  if (flag != ERROR) {
    flag = TRUE;
    cpyTetraminoFigure(&tmp, (game->figure.indexTetramino + 7) % 28);
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (tmp[i][j] &&
            (game->figure.x + j >= WIDTH || game->figure.x + j <= -1 ||
             game->figure.y + i >= HEIGHT)) {
          flag = FALSE;
        } else if (game->figure.y + i >= 0 && tmp[i][j] &&
                   game->gameInfo
                       .field[game->figure.y + i][game->figure.x + j]) {
          flag = FALSE;
        }
      }
    }

    for (int i = 0; i < 4; i++) free(tmp[i]);
    free(tmp);
  }
  return flag;
}

/**
 * @brief Изменяет общй счет игры.
 *
 * @param game Указатель на структуру Tetris.
 * @param counter Количество "сожженных" за раз линий .
 */
void changeScore(Tetris *game, int counter) {
  switch (counter) {
    case 1:
      game->gameInfo.score += 100;
      break;
    case 2:
      game->gameInfo.score += 300;
      break;
    case 3:
      game->gameInfo.score += 700;
      break;
    case 4:
      game->gameInfo.score += 1500;
      break;
    default:
      break;
  }
}

/**
 * @brief Удаляет заполненные строки и "подтягивает" строки вниз.
 *
 * @param game Указатель на структуру Tetris.
 */
void attachingFigures(Tetris *game) {
  int counter = 0;
  for (int i = 0; i < HEIGHT; i++) {
    int sum = 0;
    for (int j = 0; j < WIDTH; j++) {
      if (game->gameInfo.field[i][j]) {
        sum++;
      }
    }
    if (sum == 10) {
      counter++;
      for (int j = 0; j < WIDTH; j++) {
        game->gameInfo.field[i][j] = 0;
      }
      for (int k = i - 1; k >= 0; k--) {
        for (int j = 0; j < WIDTH; j++) {
          game->gameInfo.field[k + 1][j] = game->gameInfo.field[k][j];
        }
      }
    }
  }

  changeScore(game, counter);
  writeHighScore(game);
  updateLevel(game);
}

/**
 * @brief Копирует данные фигуры Тетрамино в игровое поле.
 *
 * @param field Указатель на указатель на поле.
 * @param indexTetramino Индекс фигуры для копирования.
 */
void cpyTetraminoFigure(int ***field, int indexTetramino) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      (*field)[i][j] = TETROMINOS[indexTetramino][i][j];
    }
  }
}

/**
 * @brief Обновляет уровень игры на основе текущего счета.
 *
 * @param game Указатель на структуру Tetris.
 */
void updateLevel(Tetris *game) {
  game->gameInfo.level =
      game->gameInfo.score / 6000 >= 1 ? 10 : 1 + game->gameInfo.score / 600;
  game->speed = game->gameInfo.level;
}

/**
 * @brief Приостанавливает или возобновляет игру.
 *
 * @param game Указатель на структуру Tetris.
 */
void gamePaused(Tetris *game) {
  if (game->gameInfo.pause == PAUSED || game->gameInfo.pause == STARTED)
    game->gameInfo.pause = game->gameInfo.pause == 1 ? PAUSED : STARTED;
}

/**
 * @brief Проверяет проигрыш.
 *
 * @param game Указатель на структуру Tetris.
 * @return true Если игра окончена.
 * @return false Если игра продолжается.
 */
bool checkLose(Tetris *game) {
  bool flagEnd = false;
  for (int i = 0; i < WIDTH; i++) {
    if (game->gameInfo.field[0][i]) {
      flagEnd = true;
    }
  }
  return flagEnd;
}

/**
 * @brief Завершает игру, сохраняя рекорд.
 *
 * @param game Указатель на структуру Tetris.
 */
void gameTerminated(Tetris *game) {
  writeHighScore(game);
  game->gameInfo.pause = QUIT;
}

/**
 * @brief Запускает игру, если она не начата.
 *
 * @param game Указатель на структуру Tetris.
 */
void gameStart(Tetris *game) {
  if (game->gameInfo.pause == NOT_STARTED) {
    game->gameInfo.pause = STARTED;
  }
}

/**
 * @brief Обрабатывает ввод пользователя в зависимости от действия и запускает
 * соответствующие функции.
 *
 * @param game Указатель на структуру Tetris.
 * @param action Действие пользователя (влево, вправо, вниз, поворот, пауза,
 * старт, завершение игры).
 * @param hold Удержание клавиши.
 */
void userInput(Tetris *game, UserAction_t action, bool hold) {
  (void)hold;
  if (game->gameInfo.pause == STARTED) {
    switch (action) {
      case Left:
        moveLeft(game);
        break;
      case Right:
        moveRight(game);
        break;
      case Down:
        updateCurrentState(game);
        break;
      case Action:
        rotate(game);
        break;
      default:
        break;
    }
  }

  switch (action) {
    case Terminate:
      gameTerminated(game);
      break;
    case Pause:
      gamePaused(game);
      break;
    case Start:
      gameStart(game);
      break;
    default:
      break;
  }
}

/**
 * @brief Считывает рекордный счет из файла.
 *
 * @param game Указатель на структуру Tetris.
 */
void readHighScore(Tetris *game) {
  FILE *f = fopen(SCORE_FILE, "r");
  if (f != NULL) {
    char line[255];
    fgets(line, 255, f);
    int s;
    sscanf(line, "%d", &s);
    game->gameInfo.high_score = s;
    fclose(f);

  } else {
    game->gameInfo.high_score = 0;
  }
}

/**
 * @brief Записывает рекордный счет в файл.
 *
 * @param game Указатель на структуру Tetris.
 */
void writeHighScore(Tetris *game) {
  if (game->gameInfo.score >= game->gameInfo.high_score) {
    game->gameInfo.high_score = game->gameInfo.score;
    FILE *f = fopen(SCORE_FILE, "w");
    if (f != NULL) {
      fprintf(f, "%d", game->gameInfo.high_score);
      fclose(f);
    }
  }
}

/**
 * @brief Освобождает память, выделенную для игрового поля и фигур.
 *
 * @param game Указатель на структуру Tetris.
 */
void freeSpace(Tetris *game) {
  for (int i = 0; i < 4; i++) free(game->figure.shape[i]);
  free(game->figure.shape);
  for (int i = 0; i < 4; i++) free(game->gameInfo.next[i]);
  free(game->gameInfo.next);
  for (int i = 0; i < HEIGHT; i++) free(game->gameInfo.field[i]);
  free(game->gameInfo.field);
}
