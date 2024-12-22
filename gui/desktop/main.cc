#include <QApplication>

#include "brickgame.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  BrickGame w;
  w.show();
  return a.exec();
}
