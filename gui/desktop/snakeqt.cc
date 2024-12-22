#include "snakeqt.h"
namespace s21 {

/**
 * @brief Конструктор класса SnakeQT.
 *
 * Инициализирует окно игры SnakeQT, устанавливает размеры окна,
 * инициализирует таймер, обновляет значение флага ошибки
 *
 * @param parent Указатель на родительский виджет (по умолчанию nullptr).
 */
SnakeQT::SnakeQT(QWidget *parent)
    : QWidget(parent), timer_(nullptr), flagError_(0) {
  srand(time(NULL));
  setFixedSize(405, 440);
  QString fontPath = ":/Snake Chan.ttf";
  int fontId = QFontDatabase::addApplicationFont(fontPath);
  if (fontId != -1) {
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    if (!fontFamilies.isEmpty()) {
      QFont tetrisFont(fontFamilies.at(0));
      setFont(tetrisFont);
    }
  } else {
    qWarning() << "Не удалось загрузить шрифт";
  }

  controller_ = new Controller(new s21::Snake()),
  flagError_ = controller_->game->GetFlagErrorGame();
  if (flagError_ != true) {
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &SnakeQT::UpdateGame);
    timer_->start(1);
  }
}

/**
 * @brief Деструктор класса SnakeQT.
 *
 * Освобождает память, используемую объектом SnakeQT,
 * и генерирует сигнал о закрытии игры.
 */
SnakeQT::~SnakeQT() { emit gameClosed(); }

/**
 * @brief Обработка нажатий клавиш.
 *
 * Эта функция вызывается при нажатии клавиш на клавиатуре.
 *
 * @param event Указатель на событие нажатия клавиши.
 */
void SnakeQT::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Escape:
      controller_->userInput(Terminate, 0);
      break;
    case Qt::Key_P:
      controller_->userInput(Pause, 0);
      break;
    case Qt::Key_Left:
      controller_->userInput(Left, 0);
      break;
    case Qt::Key_Right:
      controller_->userInput(Right, 0);
      break;
    case Qt::Key_Up:
      controller_->userInput(Up, 0);
      break;
    case Qt::Key_Down:
      controller_->userInput(Down, 0);
      break;
    case Qt::Key_R:
      controller_->userInput(Action, true);
      break;
    case Qt::Key_Return:
      controller_->userInput(Start, 0);
      break;
    default:
      QWidget::keyPressEvent(event);
      break;
  }
}

void SnakeQT::closeEvent(QCloseEvent *event) {
  emit gameClosed();
  QWidget::closeEvent(event);
}
/**
 * @brief Обработка события показа окна.
 *
 * Эта функция вызывается при показе окна и может использоваться для
 * выполнения дополнительных действий, таких как сброс состояния игры.
 *
 * @param event Указатель на событие показа.
 */
void SnakeQT::showEvent(QShowEvent *event) {
  QWidget::showEvent(event);
  ResetGame();
}
/**
 * @brief Сброс состояния игры.
 *
 * Эта функция сбрасывает игру и контроллер в начальное состояние.
 */
void SnakeQT::ResetGame() {
  if (controller_ != nullptr) delete controller_;

  controller_ = new Controller(new s21::Snake());

  flagError_ = controller_->game->GetFlagErrorGame();
  if (flagError_ != true) {
    if (timer_) {
      timer_->stop();
      timer_->start(100);
    }
  }
}

/**
 * @brief Функция обновления игры
 *
 */
void SnakeQT::UpdateGame() {
  if (controller_->game->gameInfo.pause != QUIT) {
    if (controller_->game->gameInfo.pause == STARTED) {
      controller_->updateCurrentState();
    }
    update();
  } else {
    timer_->stop();
    close();
    return;
  }
}

/**
 * @brief Обработка события рисования.
 *
 * Эта функция вызывается, когда необходимо перерисовать окно.
 *
 * @param event Указатель на событие рисования.
 */
void SnakeQT::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.fillRect(rect(), Qt::black);
  QWidget::paintEvent(event);
  DrawGame(painter);

  DrawAdditionalText(painter);
}

/**
 * @brief Основная функция отрисовки игры
 *
 * @param painter ссылка на объект отрисовщика QPainter
 */
void SnakeQT::DrawGame(QPainter &painter) {
  InitialGameBar(painter);
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      int colorIndex = controller_->game->gameInfo.field[i][j];
      if (colorIndex != 0) {
        QColor pieceColor = GetColorByIndex(colorIndex);
        painter.setBrush(pieceColor);
        painter.drawRect((j * 20) + 20, (i * 20) + 20, 20, 20);
      }
    }
  }
  DrawApple(painter);
  DrawSnake(painter);
}

/**
 * @brief Функция получения цвета по индексу
 *
 * @param colorIndex int индекс фигуры по таблице тетрамино
 */
QColor SnakeQT::GetColorByIndex(int colorIndex) const {
  switch (colorIndex) {
    case 0:

    case 12:
      return QColor(186, 143, 202);  // purpleDrab
    case 13:
      return QColor(255, 182, 193);  // pinkDrab
    case 1:
      return QColor(135, 206, 250);  // SkyBlue
    case 2:
      return QColor(0, 255, 127);  // SpringGreen
    case 3:
      return QColor(220, 20, 60);  // Crimson
    default:
      return QColor(255, 255, 255);
  }
}

/**
 * @brief Функция отрисовки сетки игрового поля
 *
 * @param painter ссылка на объект отрисовщика QPainter
 */
int SnakeQT::DrawGrid(QPainter &painter) {
  const int boardWidth = 10;
  const int boardHeight = 20;
  QPen gridPen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  gridPen.setColor(QColor(255, 255, 255, 128));

  painter.setPen(gridPen);
  QRect boardRect(drawingSize.cellWidth, drawingSize.cellHeight,
                  boardWidth * drawingSize.cellWidth,
                  boardHeight * drawingSize.cellHeight);
  painter.fillRect(boardRect, Qt::black);

  for (int row = 0; row <= 20; ++row) {
    painter.drawLine(drawingSize.cellWidth,
                     (row * drawingSize.cellHeight) + drawingSize.cellHeight,
                     10 * drawingSize.cellWidth + drawingSize.cellWidth,
                     (row * drawingSize.cellHeight) + drawingSize.cellHeight);
  }

  for (int col = 0; col <= 10; ++col) {
    painter.drawLine((col * drawingSize.cellWidth) + drawingSize.cellWidth,
                     drawingSize.cellHeight,
                     (col * drawingSize.cellWidth) + drawingSize.cellWidth,
                     20 * drawingSize.cellHeight + drawingSize.cellHeight);
  }
  return boardRect.right();
}

/**
 * @brief Функция инициализация структуры для отрисовки
 *
 */
void SnakeQT::InitialDrawingSizes() {
  drawingSize = {20, 20, 20, 3.5 * 20, 2 * 20, 150};
}

/**
 * @brief Отрисовка игрового поля, дополнительных рамок и информации
 *
 * @param painter ссылка на объект отрисовщика QPainter
 */
void SnakeQT::InitialGameBar(QPainter &painter) {
  InitialDrawingSizes();
  int gridSize = DrawGrid(painter);
  QPen pen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  pen.setColor(QColor(255, 255, 255, 255));

  painter.setPen(pen);
  DrawShapesForInfo(painter, gridSize);
}

/**
 * @brief Отрисовка сопутсвующего текста при изменении состояния игры (Перед
 * стартом, Пауза, Конец)
 *
 * @param painter ссылка на объект отрисовщика QPainter
 */
void SnakeQT::DrawAdditionalText(QPainter &painter) {
  QFont font = painter.font();
  font.setPointSizeF(font.pointSizeF() * 1.5);
  painter.setFont(font);
  QPen pen(Qt::white, 6);
  painter.setPen(pen);
  if (controller_->game->gameInfo.pause == NOT_STARTED) {
    painter.drawText(25, 135, "Press Enter");
    painter.drawText(25, 175, "to Start Game");

  } else if (controller_->game->gameInfo.pause == PAUSED) {
    painter.drawText(30, 135, "GAME PAUSED");
    painter.drawText(30, 155, "Press P Key");
    painter.drawText(30, 175, "to Continue...");

  } else if (controller_->game->gameInfo.pause == ENDED) {
    painter.drawText(55, 135, "GAME OVER");
  } else if (controller_->game->gameInfo.pause == WIN) {
    painter.drawText(25, 135, "Congratulations!");
    painter.drawText(25, 175, "YOU WIN!");
  }
}

/**
 * @brief Отрисовка игрового поля, дополнительных рамок и информации
 *
 * @param painter ссылка на объект отрисовщика QPainter
 * @param gridSize int Размер левого игрового поля
 */
void SnakeQT::DrawShapesForInfo(QPainter &painter, int &gridSize) {
  int spacing = drawingSize.spacing;
  int rectWidth = 3.5 * drawingSize.cellWidth;
  int rectHeight = 2 * drawingSize.cellHeight;
  int xOffset = gridSize + spacing;

  QRect rect1(xOffset, drawingSize.cellHeight, rectWidth * 2, rectHeight);
  painter.drawRect(rect1);
  DrawLabels(painter, rect1, "Snake");
  std::vector<std::pair<QString, QString>> data = {
      {"High\nScore", QString::number(controller_->game->gameInfo.high_score)},
      {"Score", QString::number(controller_->game->gameInfo.score)},
      {"Level", QString::number(controller_->game->gameInfo.level)}};
  for (size_t i = 0; i < data.size(); i++) {
    int yOffset =
        drawingSize.cellHeight + rectHeight * (i + 1) + spacing * (2 * i + 1);
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
void SnakeQT::DrawLabels(QPainter &painter, const QRect &rect,
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

/**
 * @brief Отрисовка яблока на игровом поле
 *
 * @param painter ссылка на объект отрисовщика QPainter
 */
void SnakeQT::DrawApple(QPainter &painter) {
  int appleX = controller_->game->gameInfo.next[0][0];
  int appleY = controller_->game->gameInfo.next[0][1];
  QColor pieceColor = GetColorByIndex(3);
  painter.setBrush(pieceColor);
  painter.drawRect((appleX * 20) + 20, (appleY * 20) + 20, 20, 20);
}

/**
 * @brief Отрисовка змейки на игровом поле
 *
 * @param painter ссылка на объект отрисовщика QPainter
 */
void SnakeQT::DrawSnake(QPainter &painter) {
  std::vector snake = controller_->game->snakeCoordinates;

  for (size_t i = 1; i < snake.size(); ++i) {
    QColor pieceColor = GetColorByIndex(1);
    painter.setBrush(pieceColor);
    painter.drawRect((snake[i].x * 20) + 20, (snake[i].y * 20) + 20, 20, 20);
  }
  QColor pieceColor = GetColorByIndex(2);
  painter.setBrush(pieceColor);
  painter.drawRect((snake[0].x * 20) + 20, (snake[0].y * 20) + 20, 20, 20);
}
}  // namespace s21
