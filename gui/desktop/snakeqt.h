#ifndef CPP3_BRICKGAME_SRC_GUI_DESKTOP_SNAKEQT_H_
#define CPP3_BRICKGAME_SRC_GUI_DESKTOP_SNAKEQT_H_

#include <QFontDatabase>
#include <QKeyEvent>
#include <QPainter>
#include <QShowEvent>
#include <QTimer>
#include <QWidget>

#include "../../brick_game/snake/controller/controller.h"
#include "tetrisqt.h"

namespace s21 {

/**
 * @brief Класс, отвечающий за отрисовку Snake в QT
 * @ingroup SnakeGame
 */
class SnakeQT : public QWidget {
  Q_OBJECT

 public:
  //    SnakeQT();
  SnakeQT(QWidget *parent = nullptr);
  ~SnakeQT();

 protected:
  void closeEvent(QCloseEvent *event) override;
  void showEvent(QShowEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

  void UpdateGame();

  void DrawGame(QPainter &painter);
  int DrawGrid(QPainter &painter);
  void DrawAdditionalText(QPainter &painter);
  void DrawShapesForInfo(QPainter &painter, int &gridSize);
  void DrawLabels(QPainter &painter, const QRect &rect, const QString &text);
  void InitialDrawingSizes();
  void InitialGameBar(QPainter &pointer);
  void DrawSnake(QPainter &pointer);
  void DrawApple(QPainter &pointer);
  void ResetGame();

  QColor GetColorByIndex(int colorIndex) const;

 signals:
  /**
   * @brief Сигнал, который вызывается при закрытии игры.
   */
  void gameClosed();

 private:
  DrawingSize drawingSize;  ///< Объект структуры DrawingSize
  Controller *controller_;  ///< Ссылка на объект класса Controller
  QTimer *timer_;  ///< Таймер для управления обновлением игры.
  int flagError_;  ///< Флаг ошибки при работе программы
};
}  // namespace s21
#endif  // CPP3_BRICKGAME_SRC_GUI_DESKTOP_SNAKEQT_H_
