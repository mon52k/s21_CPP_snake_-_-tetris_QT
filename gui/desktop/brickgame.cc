#include "brickgame.h"

#include "ui_brickgame.h"

BrickGame::BrickGame(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::brickgame),
      t(new s21::TetrisQT),
      s(new s21::SnakeQT) {
  ui->setupUi(this);

  connect(ui->ButtonTetris, &QPushButton::clicked, this,
          &BrickGame::ButtonChosedCheck);
  connect(ui->ButtonExit, &QPushButton::clicked, this,
          &BrickGame::ButtonChosedCheck);
  connect(ui->ButtonSnake, &QPushButton::clicked, this,
          &BrickGame::ButtonChosedCheck);

  connect(t, &s21::TetrisQT::gameClosed, this, &BrickGame::show);
  connect(s, &s21::SnakeQT::gameClosed, this, &BrickGame::show);
}

BrickGame::~BrickGame() { delete ui; }

void BrickGame::ButtonChosedCheck() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());

  if (button == ui->ButtonSnake) {
    hide();
    s->show();
  } else if (button == ui->ButtonTetris) {
    hide();
    t->show();
  } else if (button == ui->ButtonExit) {
    close();
  }
}
