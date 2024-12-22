#include "tetrisqt.h"
namespace s21 {
/**
 * @brief Конструктор класса TetrisQT.
 *
 * Инициализирует окно игры Tetris, устанавливает размеры окна,
 * инициализирует таймер
 *
 * @param parent Указатель на родительский виджет (по умолчанию nullptr).
 */
TetrisQT::TetrisQT(QWidget *parent) : QWidget(parent), counter_(1) {
  srand(time(NULL));
  setFixedSize(405, 440);
  flagError_ = ::initialGame(&game_);
  ::userInput(&game_, Start, 0);
  if (flagError_ != ERROR) {
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &TetrisQT::GameLoop);
    timer_->start(READ_DELAY);
  }
}

/**
 * @brief Деструктор класса TetrisQT.
 *
 * Освобождает память, используемую объектом TetrisQT,
 * и генерирует сигнал о закрытии игры.
 */
TetrisQT::~TetrisQT() {
  emit gameClosed();
  ::freeSpace(&game_);
}

/**
 * @brief Обработка нажатий клавиш.
 *
 * Эта функция вызывается при нажатии клавиш на клавиатуре.
 *
 * @param event Указатель на событие нажатия клавиши.
 */
void TetrisQT::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Escape:
      ::userInput(&game_, Terminate, 0);
      break;
    case Qt::Key_P:
      ::userInput(&game_, Pause, 0);
      break;
    case Qt::Key_Left:
      ::userInput(&game_, Left, 0);
      break;
    case Qt::Key_Right:
      ::userInput(&game_, Right, 0);
      break;
    case Qt::Key_R:
      ::userInput(&game_, Action, 0);
      break;
    case Qt::Key_Down:
      ::userInput(&game_, Down, 0);
      break;
    case Qt::Key_Return:
      ::userInput(&game_, Start, 0);
      break;
    default:
      QWidget::keyPressEvent(event);
      break;
  }
}

/**
 * @brief Основной цикл игры
 *  С помощью counter с определенной скоростью
 *  вызывает Обновление состояние игры
 */
void TetrisQT::GameLoop() {
  if (game_.gameInfo.pause != QUIT) {
    if (game_.gameInfo.pause == STARTED) {
      if (counter_ >= (1.55 - game_.speed * SPEED_RATE)) {
        ::updateCurrentState(&game_);
        counter_ = 0.0;
      }
      counter_ += READ_DELAY * 0.001;
    }
    update();
  } else {
    timer_->stop();
    close();
    return;
  }
}

/**
 * @brief Обработка события закрытия окна.
 *
 * Генерирует сигнал о закрытии игры и вызывает стандартное
 * поведение закрытия.
 *
 * @param event Указатель на событие закрытия.
 */
void TetrisQT::closeEvent(QCloseEvent *event) {
  emit gameClosed();
  QWidget::closeEvent(event);
}

/**
 * @brief Обработка события показа окна.
 *
 * Эта функция вызывается, когда окно становится видимым.
 *
 * @param event Указатель на событие показа.
 */
void TetrisQT::showEvent(QShowEvent *event) {
  QWidget::showEvent(event);
  ResetGame();
}

/**
 * @brief Сброс игры.
 *
 * Эта функция сбрасывает параметры игры к начальным значениям
 * и перезапускает таймер.
 */
void TetrisQT::ResetGame() {
  if (timer_->isActive()) {
    timer_->stop();
  }

  ::freeSpace(&game_);
  flagError_ = ::initialGame(&game_);

  counter_ = 0.0;
  timer_->start(READ_DELAY);

  update();
}

/**
 * @brief Отрисовывает Фигуры на поле
 *
 * @param painter ссылка на объект отрисовщика QPainter
 */
void TetrisQT::DrawGame(QPainter &painter) {
  InitialGameBar(painter);
  DrawNext(painter);

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      int colorIndex = game_.gameInfo.field[i][j];
      if (colorIndex != 0) {
        QColor pieceColor = GetColorByIndex(colorIndex);
        painter.setBrush(pieceColor);
        painter.drawRect((j * 20) + 20, (i * 20) + 20, 20, 20);
      }
    }
  }

  if (game_.gameInfo.pause != ENDED) {
    DrawFigure(painter);
  }
}

/**
 * @brief Отрисовывает текущую фигуру на игровом поле.
 *
 * @param painter ссылка на объект отрисовщика QPainter
 */
void TetrisQT::DrawFigure(QPainter &painter) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (game_.figure.shape[y][x] && game_.figure.y + y >= 0) {
        QColor pieceColor =
            GetColorByIndex(game_.figure.indexTetramino % 7 + 1);
        painter.setBrush(pieceColor);
        painter.drawRect(((game_.figure.x + x) * 20) + 20,
                         ((game_.figure.y + y) * 20) + 20, 20, 20);
      }
    }
  }
}

/**
 * @brief Отрисовывает следующую фигуру на поле данных, которая будет после
 * текущей.
 *
 * @param painter ссылка на объект отрисовщика QPainter
 */
void TetrisQT::DrawNext(QPainter &painter) {
  int pieceStartX = 320;
  int pieceStartY = 60;
  for (int i = 2; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game_.gameInfo.next[i][j]) {
        QColor pieceColor = GetColorByIndex(game_.figure.indexNext % 7 + 1);
        painter.setBrush(pieceColor);
        painter.drawRect((pieceStartX + j * 20), (pieceStartY + i * 20), 20,
                         20);
      }
    }
  }
}

/**
 * @brief Обработка события рисования.
 *
 * Эта функция вызывается, когда необходимо перерисовать окно.
 *
 * @param event Указатель на событие рисования.
 */
void TetrisQT::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.fillRect(rect(), Qt::black);
  QWidget::paintEvent(event);

  DrawGame(painter);
  DrawAdditionalText(painter);
}

/**
 * @brief Отрисовка сопутсвующего текста при изменении состояния игры (Перед
 * стартом, Пауза, Конец)
 *
 * @param painter ссылка на объект отрисовщика QPainter
 */
void TetrisQT::DrawAdditionalText(QPainter &painter) {
  QFont font = painter.font();                  // Get the current font
  font.setPointSizeF(font.pointSizeF() * 1.5);  // Double the current size
  painter.setFont(font);
  QPen pen(Qt::white, 6);
  painter.setPen(pen);
  if (game_.gameInfo.pause == NOT_STARTED) {
    painter.drawText(40, 135, "Press Enter");
    painter.drawText(80, 175, "to Start Game");

  } else if (game_.gameInfo.pause == PAUSED) {
    painter.drawText(40, 135, "GAME PAUSED");
    painter.drawText(70, 155, "Press P Key");
    painter.drawText(100, 175, "to Continue...");

  } else if (game_.gameInfo.pause == ENDED) {
    painter.drawText(60, 135, "GAME OVER");
  }
}

/**
 * @brief Функция получения цвета по индексу
 *
 * @param colorIndex int индекс фигуры по таблице тетрамино
 */
QColor TetrisQT::GetColorByIndex(int colorIndex) const {
  switch (colorIndex) {
    case 1:
      return QColor(255, 0, 0);  // Red
    case 2:
      return QColor(255, 165, 0);  // Orange
    case 3:
      return QColor(255, 255, 0);  // Yellow
    case 4:
      return QColor(255, 192, 203);  // Pink
    case 5:
      return QColor(0, 255, 0);  // Green
    case 6:
      return QColor(0, 0, 255);  // Blue
    case 7:
      return QColor(128, 0, 128);  // Purple
    default:
      return QColor(255, 255, 255);
  }
}

/**
 * @brief Функция отрисовки сетки игрового поля
 *
 * @param painter ссылка на объект отрисовщика QPainter
 */
int TetrisQT::DrawGrid(QPainter &painter) {
  const int boardWidth = 10;
  const int boardHeight = 20;
  QPen gridPen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  gridPen.setColor(QColor(255, 255, 255, 128));

  painter.setPen(gridPen);
  QRect boardRect(drawingSize_.cellWidth, drawingSize_.cellHeight,
                  boardWidth * drawingSize_.cellWidth,
                  boardHeight * drawingSize_.cellHeight);
  painter.fillRect(boardRect, Qt::black);
  for (int row = 0; row <= 20; ++row) {
    painter.drawLine(drawingSize_.cellWidth,
                     (row * drawingSize_.cellHeight) + drawingSize_.cellHeight,
                     10 * drawingSize_.cellWidth + drawingSize_.cellWidth,
                     (row * drawingSize_.cellHeight) + drawingSize_.cellHeight);
  }

  for (int col = 0; col <= 10; ++col) {
    painter.drawLine((col * drawingSize_.cellWidth) + drawingSize_.cellWidth,
                     drawingSize_.cellHeight,
                     (col * drawingSize_.cellWidth) + drawingSize_.cellWidth,
                     20 * drawingSize_.cellHeight + drawingSize_.cellHeight);
  }
  return boardRect.right();
}

/**
 * @brief Функция инициализация структуры для отрисовки
 *
 */
void TetrisQT::InitialDrawingSizes() {
  drawingSize_ = {20, 20, 20, 3.5 * 20, 2 * 20, 150};
}

/**
 * @brief Отрисовка игрового поля, дополнительных рамок и информации
 *
 * @param painter ссылка на объект отрисовщика QPainter
 */
void TetrisQT::InitialGameBar(QPainter &painter) {
  InitialDrawingSizes();
  int gridSize = DrawGrid(painter);
  QPen pen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  pen.setColor(QColor(255, 255, 255, 255));

  painter.setPen(pen);
  DrawShapesForInfo(painter, gridSize);
}

/**
 * @brief Отрисовка игрового поля, дополнительных рамок и информации
 *
 * @param painter ссылка на объект отрисовщика QPainter
 * @param gridSize int Размер левого игрового поля
 */
void TetrisQT::DrawShapesForInfo(QPainter &painter, int &gridSize) {
  int spacing = drawingSize_.spacing;
  int rectWidth = 3.5 * drawingSize_.cellWidth;
  int rectHeight = 2 * drawingSize_.cellHeight;
  int xOffset = gridSize + spacing;
  QRect rect1(xOffset, drawingSize_.cellHeight, rectWidth * 2, rectHeight);
  painter.drawRect(rect1);
  DrawLabels(painter, rect1, "Tetris");

  QRect rect2(xOffset, drawingSize_.cellHeight + rectHeight + spacing,
              rectWidth, rectHeight * 1.5 + spacing);
  painter.drawRect(rect2);
  DrawLabels(painter, rect2, "Next");

  std::vector<std::pair<QString, QString>> data = {
      {"High\nScore", QString::number(game_.gameInfo.high_score)},
      {"Score", QString::number(game_.gameInfo.score)},
      {"Level", QString::number(game_.gameInfo.level)}};
  for (int i = 0; i < 3; i++) {
    int yOffset = drawingSize_.cellHeight + rectHeight * ((i + 1) * 1.5 + 1) +
                  spacing * (3 + i);
    QRect rect1(xOffset, yOffset, rectWidth, rectHeight * 1.5);
    painter.fillRect(rect1, Qt::black);
    painter.drawRect(rect1);
    DrawLabels(painter, rect1, data[i].first);

    QRect rect2(xOffset + rectWidth + 10, yOffset, rectWidth, rectHeight * 1.5);
    painter.fillRect(rect2, Qt::black);
    painter.drawRect(rect2);
    DrawLabels(painter, rect2, data[i].second);
  }
}

/**
 * @brief Отрисовка названий и численных значений параметров игры
 *
 * @param painter ссылка на объект отрисовщика QPainter
 * @param rect - ссылка на объект QRect (прямоугольник)
 * @param text - текст QString для отрисовки
 */
void TetrisQT::DrawLabels(QPainter &painter, const QRect &rect,
                          const QString &text) {
  QFontMetrics metrics(painter.font());
  QStringList lines = text.split('\n');
  int totalHeight = lines.size() * metrics.height();
  int totalWidth = 0;
  for (const QString &line : lines) {
    totalWidth = qMax(totalWidth, metrics.horizontalAdvance(line));
  }

  int x = rect.left() + (rect.width() - totalWidth) / 2;
  int y = rect.top() + (rect.height() + totalHeight) / 2;
  int lineY = y - (lines.size() - 1) * metrics.height() / 2;

  if (lineY + totalHeight > rect.bottom()) {
    lineY = rect.bottom() - totalHeight;
  }
  for (const QString &line : lines) {
    painter.drawText(x, lineY, line);
    lineY += metrics.height();
  }
}
}  // namespace s21
