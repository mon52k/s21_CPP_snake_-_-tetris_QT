#include "tetris_frontend.h"

/**
 * @brief Инициализирует игру, обрабатывает ввод и выводит графику в игровом
 * цикле.
 *
 * @return int Статус программы (0 - успех, 1 - ошибка).
 */
int start() {
  initialize_ncurses();
  Tetris game;

  if (initialGame(&game) == ERROR) {
    return 1;
  }
  double counter = 1;

  while (game.gameInfo.pause == NOT_STARTED) {
    mvprintw(6, 2, "GAME READY");
    mvprintw(8, 5, "Press ENTER");
    mvprintw(10, 8, "to Start");
    handleInput(&game);
    draw(&game);
  }

  while (game.gameInfo.pause != QUIT) {
    handleInput(&game);
    if (game.gameInfo.pause == STARTED) {
      if (counter >= (1.55 - game.speed * SPEED_RATE)) {
        updateCurrentState(&game);
        counter = 0.0;
      }
      counter += READ_DELAY * 0.001;
    }
    draw(&game);
  }
  clearField(&game);
  endwin();
  return 0;
}

/**
 * @brief Инициализирует цветовые схемы для NCURSES.
 */
void init_colors() {
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_YELLOW, COLOR_RED);
  init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(4, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(5, COLOR_GREEN, COLOR_BLACK);
  init_pair(6, COLOR_BLUE, COLOR_BLUE);
  init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
}

/**
 * @brief Инициализация NCURSES и необходимых настроек для интерфейса игры.
 */
void initialize_ncurses() {
  initscr();
  init_colors();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  timeout(timet);
}

/**
 * @brief Отображает начальную игровую панель с информацией о следующей фигуре,
 * счёте, уровне и ника разработчика.
 */
void initialGamebar() {
  // Tetris Label
  print_rectangle(1, 3, 22, 37);
  mvprintw(2, 26, "Tetris");

  // Next
  print_rectangle(5, 7, 22, 28);
  mvprintw(6, 23, "Next");

  // High Score
  print_rectangle(8, 11, 22, 28);
  mvprintw(9, 23, "HIGH");
  mvprintw(10, 23, "Score");
  print_rectangle(8, 11, 29, 37);

  // Score
  print_rectangle(12, 14, 22, 28);
  mvprintw(13, 23, "Score");
  print_rectangle(12, 14, 29, 37);

  // Level
  print_rectangle(15, 17, 22, 28);
  mvprintw(16, 23, "Level");
  print_rectangle(15, 17, 29, 37);

  print_rectangle(18, 20, 24, 36);
  mvprintw(19, 26, "zubatshr");
}

/**
 * @brief Отрисовывает текущее состояние игры, включая зафиксированные фигуры на
 * поле и игровую панель.
 *
 * @param game Указатель на структуру Tetris.
 */
void draw(Tetris *game) {
  clear();
  draw_next(game);
  print_rectangle(0, 21, 0, 21);
  print_rectangle(0, 21, 21, 38);
  initialGamebar();
  mvprintw(10, 31, "%d", game->gameInfo.high_score);
  mvprintw(13, 31, "%d", game->gameInfo.score);
  mvprintw(16, 31, "%d", game->gameInfo.level);

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (game->gameInfo.field[i][j] != 0) {
        attron(COLOR_PAIR(game->gameInfo.field[i][j]));
        mvaddch(i + 1, j * 2 + 1, ACS_CKBOARD);
        mvaddch(i + 1, j * 2 + 2, ACS_CKBOARD);
        attroff(COLOR_PAIR(game->gameInfo.field[i][j]));
      }
    }
  }

  if (game->gameInfo.pause == ENDED) {
    mvprintw(2, 26, "GAME OVER");
  } else {
    draw_figure(game);
  }

  if (game->gameInfo.pause == PAUSED) {
    mvprintw(2, 25, "GAME PAUSED");
    mvprintw(8, 3, "Press P Key");
    mvprintw(10, 7, "to Continue");
  }
}

/**
 * @brief Отрисовывает текущую фигуру на игровом поле.
 *
 * @param game Указатель на структуру Tetris.
 */
void draw_figure(Tetris *game) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->figure.shape[i][j] == 1 && game->figure.y + i >= 0) {
        attron(COLOR_PAIR(game->figure.indexTetramino % 7 + 1));
        mvaddch((game->figure.y + i) + 1, (game->figure.x + j) * 2 + 1,
                ACS_CKBOARD);
        mvaddch((game->figure.y + i) + 1, (game->figure.x + j) * 2 + 2,
                ACS_CKBOARD);
        attroff(COLOR_PAIR(game->figure.indexTetramino % 7 + 1));

        attroff(game->figure.indexTetramino % 7 + 1);
      }
    }
  }
}

/**
 * @brief Отрисовывает следующую фигуру на поле данных, которая будет после
 * текущей.
 *
 * @param game Указатель на структуру Tetris.
 */
void draw_next(Tetris *game) {
  int m = 5;
  int n = 30;
  for (int i = 2; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->gameInfo.next[i][j] == 1) {
        attron(COLOR_PAIR(game->figure.indexNext % 7 + 1));

        mvaddch(m, n, ACS_CKBOARD);
        mvaddch(m, n + 1, ACS_CKBOARD);
        attroff(COLOR_PAIR(game->figure.indexNext % 7 + 1));
      }
      n += 2;
    }

    n = 30;
    m++;
  }
}

/**
 * @brief Обрабатывает пользовательский ввод для управления игрой.
 *
 * @param game Указатель на структуру Tetris.
 */
void handleInput(Tetris *game) {
  int ch = getch();
  switch (ch) {
    case 'q':
      userInput(game, Terminate, 0);
      break;
    case 'p':
      userInput(game, Pause, 0);
      break;
    case KEY_LEFT:
      userInput(game, Left, 0);
      break;
    case KEY_RIGHT:
      userInput(game, Right, 0);
      break;
    case 'r':
      userInput(game, Action, 0);
      break;
    case KEY_DOWN:
      userInput(game, Down, 0);
      break;
    case '\n':
      userInput(game, Start, 0);
      break;
  }
}

/**
 * @brief Отрисовывает прямоугольник с заданными координатами.
 *
 * @param top_y Верхняя координата по оси Y.
 * @param bottom_y Нижняя координата по оси Y.
 * @param left_x Левая координата по оси X.
 * @param right_x Правая координата по оси X.
 */
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  mvaddch(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) mvaddch(top_y, i, ACS_HLINE);
  mvaddch(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    mvaddch(i, left_x, ACS_VLINE);
    mvaddch(i, right_x, ACS_VLINE);
  }

  mvaddch(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) mvaddch(bottom_y, i, ACS_HLINE);
  mvaddch(bottom_y, i, ACS_LRCORNER);
}

/**
 * @brief Очищает игровое поле после завершения игры.
 *
 * @param game Указатель на структуру Tetris.
 */
void clearField(Tetris *game) {
  freeSpace(game);
  printf("\033[H\033[J");
}