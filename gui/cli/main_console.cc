#include "main_console.h"

namespace s21 {

/**
 * @brief Инициализация NCURSES и необходимых настроек для интерфейса игры.
 */

/**
 * @brief Функция старта выбранной игры
 *
 */
bool startChoosedGame(int *choosedPoint) {
  bool flag = false;
  if (*choosedPoint == 0) {
    Snake game;
    Controller controller(&game);
    SnakeConsole snakeConsole(&controller);
    if (!game.GetFlagErrorGame()) {
      snakeConsole.start();

    } else {
      flag = true;
    }
    clear();
  } else if (*choosedPoint == 1) {
    flag = (bool)start();
    clear();

  } else {
    *choosedPoint = -1;
  }
  return flag;
}

/**
 * @brief Функция выбора игры
 *
 */
bool handleInputMenu(int *choosedPoint) {
  bool flag = false;
  int ch = getch();
  switch (ch) {
    case KEY_UP:
      (*choosedPoint)--;
      *choosedPoint < 0 ? *choosedPoint = *choosedPoint + 3 : 0;
      break;
    case KEY_DOWN:
      (*choosedPoint)++;
      *choosedPoint = *choosedPoint % 3;
      break;
    case '\n':
      flag = startChoosedGame(choosedPoint);
  }
  return flag;
}

/**
 * @brief Функция отрисовки меню
 *
 */
void drawMenu(int choosedPoint) {
  printRectangle(6, 8, 10, 28);
  switch (choosedPoint) {
    case 0:
      attron(A_REVERSE);
      mvprintw(7, 11, "%s", "      SNAKE      ");
      attroff(A_REVERSE);
      mvprintw(11, 11, "%s", "      TETRIS      ");
      mvprintw(15, 11, "%s", "      EXIT      ");
      break;

    case 1:
      mvprintw(7, 11, "%s", "      SNAKE      ");
      attron(A_REVERSE);
      mvprintw(11, 11, "%s", "      TETRIS      ");
      attroff(A_REVERSE);
      mvprintw(15, 11, "%s", "      EXIT       ");
      break;

    default:
      mvprintw(7, 11, "%s", "      SNAKE      ");
      mvprintw(11, 11, "%s", "      TETRIS      ");
      attron(A_REVERSE);
      mvprintw(15, 11, "%s", "      EXIT      ");
      attroff(A_REVERSE);
      break;
  }

  printRectangle(10, 12, 10, 28);
  printRectangle(14, 16, 10, 28);
  printRectangle(0, 21, 0, 38);
}

/**
 * @brief Точка входа в программу. Вызов функции запуска игры.
 *
 * @return int Статус выхода программы (0 - успех, 1 - ошибка).
 */

}  // namespace s21

int main() {
  int flag = 0;
  int choosedPoint = 0;
  srand((unsigned)time(NULL));

  s21::initializeNcurses();
  while (choosedPoint != -1) {
    if (s21::handleInputMenu(&choosedPoint)) {
      mvprintw(2, 2, "%s", "Error while allocating memory for the game");
      refresh();
      usleep(3000000);
      choosedPoint = -1;
      flag = 1;
    } else {
      s21::drawMenu(choosedPoint);
    }
  }

  endwin();
  return flag;
}