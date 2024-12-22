#ifndef CPP3_BRICKGAME_SRC_GUI_DESKTOP_BRICKGAME_H_
#define CPP3_BRICKGAME_SRC_GUI_DESKTOP_BRICKGAME_H_

#include <QMainWindow>
#include <QPushButton>

#include "snakeqt.h"
#include "tetrisqt.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class brickgame;
}
QT_END_NAMESPACE

class BrickGame : public QMainWindow {
  Q_OBJECT

 public:
  BrickGame(QWidget *parent = nullptr);
  ~BrickGame();

 private slots:
  void ButtonChosedCheck();

 private:
  Ui::brickgame *ui;
  s21::TetrisQT *t;
  s21::SnakeQT *s;
  QPushButton *ButtonTetris;
  QPushButton *ButtonExit;
  QPushButton *ButtonSnake;
};
#endif  // CPP3_BRICKGAME_SRC_GUI_DESKTOP_BRICKGAME_H_
