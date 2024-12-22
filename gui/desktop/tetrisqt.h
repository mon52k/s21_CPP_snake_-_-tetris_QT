#ifndef CPP3_BRICKGAME_SRC_GUI_DESKTOP_TETRISQT_H_
#define CPP3_BRICKGAME_SRC_GUI_DESKTOP_TETRISQT_H_

#define SPEED_RATE 0.125
#define READ_DELAY 50

#include <QFontMetrics>
#include <QKeyEvent>
#include <QPainter>
#include <QRect>
#include <QShowEvent>
#include <QTimer>
#include <QWidget>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../brick_game/tetris/tetris_backend.h"
#ifdef __cplusplus
}
#endif
namespace s21 {

/**
 * @brief Структура данных для хранения размеров клеток и интервала между ними.
 */
typedef struct {
  int cellWidth;
  int cellHeight;
  int spacing;
  double rectWidth;
  int rectHeight;
  int xOffset;
} DrawingSize;

/**
 * @brief Класс, отвечающий за отрисовку Tetris в QT
 * @ingroup TetrisGame
 */
class TetrisQT : public QWidget {
  Q_OBJECT

 public:
  TetrisQT(QWidget *parent = nullptr);
  ~TetrisQT();

 signals:
  /**
   * @brief Сигнал, который вызывается при закрытии игры.
   */
  void gameClosed();

 protected:
  void closeEvent(QCloseEvent *event) override;
  void showEvent(QShowEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void ResetGame();

  void DrawGame(QPainter &painter);
  void DrawFigure(QPainter &painter);
  void DrawNext(QPainter &painter);
  void DrawAdditionalText(QPainter &painter);
  int DrawGrid(QPainter &painter);

  void InitialGameBar(QPainter &pointer);
  void InitialDrawingSizes();
  void DrawShapesForInfo(QPainter &painter, int &gridSize);
  void DrawLabels(QPainter &painter, const QRect &rect, const QString &text);
  QColor GetColorByIndex(int colorIndex) const;
  void GameLoop();

 private:
  DrawingSize drawingSize_;  ///< Объект структуры DrawingSize

  Tetris game_;  ///< Объект игры Tetris
  int flagError_;  ///< Флаг ошибки при работе программы
  QTimer *timer_;  ///< Таймер для управления обновлением игры.
  double counter_;  ///< Счетчик для обработки скорости игры
};
}  // namespace s21

#endif  // CPP3_BRICKGAME_SRC_GUI_DESKTOP_TETRISQT_H_
