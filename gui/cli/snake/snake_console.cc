#include "snake_console.h"
namespace s21 {
/**
 * @brief Конструктор класса SnakeConsole
 */
SnakeConsole::SnakeConsole(Controller *controller) : controller(controller) {
  start_color();
  init_pair(11, COLOR_RED, COLOR_RED);
  init_pair(15, COLOR_BLUE, COLOR_BLACK);
  init_pair(14, COLOR_BLACK, COLOR_BLUE);
  init_pair(12, COLOR_BLACK, COLOR_GREEN);
  init_pair(13, COLOR_GREEN, COLOR_GREEN);
}

/**
 * @brief Деструктор класса SnakeConsole
 */
SnakeConsole::~SnakeConsole() noexcept { printf("\033[H\033[J"); }

/**
 * @brief Основная функция работы класса
 */
void SnakeConsole::start() {
  while (controller->game->gameInfo.pause == NOT_STARTED) {
    mvprintw(6, 2, "GAME READY");
    mvprintw(8, 5, "Press ENTER");
    mvprintw(10, 8, "to Start");
    HandleInput();
    Draw();
  }

  while (controller->game->gameInfo.pause != QUIT) {
    timeout(50);
    HandleInput();
    if (controller->game->gameInfo.pause == STARTED) {
      controller->updateCurrentState();
    }
    Draw();
  }
  endwin();
}

/**
 * @brief Отображает начальную игровую панель с дополнительной информацией
 */
void SnakeConsole::InitialGamebar() {
  // Snake Label
  printRectangle(1, 3, 22, 37);
  mvprintw(2, 27, "SNAKE");

  // High Score
  printRectangle(5, 8, 22, 28);
  mvprintw(6, 23, "HIGH");
  mvprintw(7, 23, "Score");
  printRectangle(5, 8, 29, 37);

  // Score
  printRectangle(9, 11, 22, 28);
  mvprintw(10, 23, "Score");
  printRectangle(9, 11, 29, 37);

  printRectangle(18, 20, 24, 36);
  mvprintw(19, 26, "zubatshr");

  // Level
  printRectangle(13, 15, 22, 28);
  mvprintw(14, 23, "Level");
  printRectangle(13, 15, 29, 37);
}

/**
 * @brief Отрисовывает текущее состояние игры
 *
 */
void SnakeConsole::Draw() {
  clear();
  printRectangle(0, 21, 0, 21);
  printRectangle(0, 21, 21, 38);
  InitialGamebar();
  mvprintw(7, 31, "%d", controller->game->gameInfo.high_score);
  mvprintw(10, 31, "%d", controller->game->gameInfo.score);
  mvprintw(14, 31, "%d", controller->game->gameInfo.level);

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (controller->game->gameInfo.field[i][j] != 0) {
        attron(COLOR_PAIR(controller->game->gameInfo.field[i][j]));
        mvaddch(i + 1, j * 2 + 1, ACS_CKBOARD);
        mvaddch(i + 1, j * 2 + 2, ACS_CKBOARD);
        attroff(COLOR_PAIR(controller->game->gameInfo.field[i][j]));
      }
    }
  }
  DrawApple();
  DrawSnake();
  if (controller->game->gameInfo.pause == LOSED) {
    mvprintw(9, 7, "GAME OVER!");

    mvprintw(2, 26, "GAME OVER");
  }

  if (controller->game->gameInfo.pause == WIN) {
    mvprintw(8, 3, "Congratulations!");
    mvprintw(2, 26, "YOU WIN");
  }
  if (controller->game->gameInfo.pause == PAUSED) {
    mvprintw(2, 25, "GAME PAUSED");
    mvprintw(8, 3, "Press P Key");
    mvprintw(10, 7, "to Continue");
  }
}

/**
 * @brief Обрабатывает пользовательский ввод для управления игрой.
 *
 */
void SnakeConsole::HandleInput() {
  int ch = getch();
  switch (ch) {
    case 'q':
      controller->userInput(Terminate, 0);
      break;
    case 'p':
      controller->userInput(Pause, 0);
      break;
    case KEY_LEFT:
      controller->userInput(Left, 0);
      break;
    case KEY_RIGHT:
      controller->userInput(Right, 0);
      break;
    case KEY_UP:
      controller->userInput(Up, 0);
      break;
    case KEY_DOWN:
      controller->userInput(Down, 0);
      break;
    case 'r':
      controller->userInput(Action, true);
      break;
    case '\n':
      controller->userInput(Start, 0);
      break;
    default:
      break;
  }
}

/**
 * @brief Инициализация консольного интерфейса.
 *
 */
void initializeNcurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  timeout(timet);
}

/**
 * @brief Отрисовывает прямоугольник с заданными координатами.
 *
 * @param top_y Верхняя координата по оси Y.
 * @param bottom_y Нижняя координата по оси Y.
 * @param left_x Левая координата по оси X.
 * @param right_x Правая координата по оси X.
 */
void printRectangle(int top_y, int bottom_y, int left_x, int right_x) {
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
 * @brief Отрисовывает положение змейки
 *
 */
void SnakeConsole::DrawSnake() {
  std::vector snake = controller->game->snakeCoordinates;

  for (size_t i = 1; i < snake.size(); ++i) {
    attron(COLOR_PAIR(14));
    mvaddch(snake[i].y + 1, snake[i].x * 2 + 1, ACS_CKBOARD);
    mvaddch(snake[i].y + 1, snake[i].x * 2 + 2, ACS_CKBOARD);
    attroff(COLOR_PAIR(14));
  }
  attron(COLOR_PAIR(15));
  mvaddch(snake.front().y + 1, snake.front().x * 2 + 1, ACS_CKBOARD);
  mvaddch(snake.front().y + 1, snake.front().x * 2 + 2, ACS_CKBOARD);
  attroff(COLOR_PAIR(15));
}

/**
 * @brief Отрисовывает положение яблока
 *
 */
void SnakeConsole::DrawApple() noexcept {
  int appleX = controller->game->gameInfo.next[0][0];
  int appleY = controller->game->gameInfo.next[0][1];

  attron(COLOR_PAIR(11));
  mvaddch(appleY + 1, appleX * 2 + 1, ACS_CKBOARD);
  mvaddch(appleY + 1, appleX * 2 + 2, ACS_CKBOARD);
  attroff(COLOR_PAIR(11));
}
}  // namespace s21