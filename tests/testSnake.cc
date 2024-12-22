

#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>

#include "../brick_game/snake/controller/controller.h"
#include "../brick_game/snake/model/snake.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  srand(static_cast<unsigned>(time(0)));

  return RUN_ALL_TESTS();
}
namespace s21{
class SnakeGameTest : public ::testing::Test {
 protected:
  void SetUp() override { srand(static_cast<unsigned>(time(0))); }
};

TEST_F(SnakeGameTest, InitialGame1) {
  EXPECT_NO_THROW(Snake game);
}

TEST_F(SnakeGameTest, InitialGame2) {
  Snake game;
  EXPECT_NO_THROW(Controller controller (&game));
  EXPECT_TRUE(game.gameInfo.pause == NOT_STARTED);
}

TEST_F(SnakeGameTest, startGame) {
  Snake game;
  Controller controller (&game);
  controller.userInput(Start,0);
  EXPECT_TRUE(game.gameInfo.pause == STARTED);
}

TEST_F(SnakeGameTest, pauseGame) {
  Snake game;
  Controller controller (&game);
  controller.userInput(Pause,0);
  EXPECT_TRUE(game.gameInfo.pause == NOT_STARTED);
  controller.userInput(Start,0);
  controller.userInput(Pause,0);
  EXPECT_TRUE(game.gameInfo.pause == PAUSED);
}

TEST_F(SnakeGameTest, terminateGame) {
  Snake game;
  Controller controller (&game);
  controller.userInput(Terminate,0);
  EXPECT_TRUE(game.gameInfo.pause == QUIT);
}

TEST_F(SnakeGameTest, winGame) {
  Snake game;
  Controller controller (&game);
  game.gameInfo.score = 196;
  game.MovingSnake();
  EXPECT_TRUE(game.gameInfo.pause == WIN);
}

TEST_F(SnakeGameTest, loseGame1) {
  Snake game;
  Controller controller (&game);
  game.GameStart();
Snake::SnakeElement newElement = {0, 0};
   auto iter = game.snakeCoordinates.cbegin();
  game.snakeCoordinates.insert(iter, newElement);
  controller.userInput(Up, 0);
  game.MovingSnake();
  EXPECT_TRUE(game.gameInfo.pause == LOSED);
}
TEST_F(SnakeGameTest, loseGame2) {
  Snake game;
  Controller controller (&game);
  game.GameStart();
Snake::SnakeElement newElement = {0, 0};

   auto iter = game.snakeCoordinates.cbegin();
  game.snakeCoordinates.insert(iter, newElement);
  controller.userInput(Left, 0);
  game.MovingSnake();
  EXPECT_TRUE(game.gameInfo.pause == LOSED);
}
TEST_F(SnakeGameTest, loseGame3) {
  Snake game;
  Controller controller (&game);
  game.GameStart();

   auto iter = game.snakeCoordinates.cbegin();
Snake::SnakeElement newElement = {WIDTH - 1, HEIGHT - 1};

  game.snakeCoordinates.insert(iter, newElement);
    controller.userInput(Right, 0);
  game.MovingSnake();
  EXPECT_TRUE(game.gameInfo.pause == LOSED);
}
TEST_F(SnakeGameTest, loseGame4) {
  Snake game;
  Controller controller (&game);
  game.SetDirection(Left);
  game.GameStart();
Snake::SnakeElement newElement = {WIDTH - 1, HEIGHT - 1};

   auto iter = game.snakeCoordinates.cbegin();
  game.snakeCoordinates.insert(iter,newElement);
    controller.userInput(Down, 0);

  game.MovingSnake();
  EXPECT_TRUE(game.gameInfo.pause == LOSED);
}
TEST_F(SnakeGameTest, loseGame5) {
  Snake game;
  Controller controller (&game);
  game.GameStart();

  game.snakeCoordinates = { {1, 1}, {2, 1}, {2, 0}, {1, 0}, {0,0} };
  controller.userInput(Up, 0);
  game.MovingSnake();
  game.MovingSnake();
  EXPECT_TRUE(game.gameInfo.pause == LOSED);

}

TEST_F(SnakeGameTest, MoveDown) {
  Snake game;
  Controller controller (&game);
  game.SetDirection(Up);
  game.MoveDown();
  EXPECT_FALSE(game.GetDirection() == Down);

  game.SetDirection(Right);
  game.MoveDown();
  EXPECT_TRUE(game.GetDirection() == Down);
}

TEST_F(SnakeGameTest, MoveUp) {
  Snake game;
  Controller controller (&game);
  game.SetDirection(Down);
  game.MoveUp();
  EXPECT_FALSE(game.GetDirection() == Up);

  game.SetDirection(Right);
  game.MoveUp();
  EXPECT_TRUE(game.GetDirection() == Up);
}

TEST_F(SnakeGameTest, MoveLeft) {
  Snake game;
  Controller controller (&game);
  game.SetDirection(Right);
  game.MoveLeft();
  EXPECT_FALSE(game.GetDirection() == Left);

  game.SetDirection(Up);
  game.MoveLeft();
  EXPECT_TRUE(game.GetDirection() == Left);
}

TEST_F(SnakeGameTest, MoveRight) {
  Snake game;
  Controller controller (&game);
  game.SetDirection(Left);
  game.MoveRight();
  EXPECT_FALSE(game.GetDirection() == Right);

  game.SetDirection(Up);
  game.MoveRight();
  EXPECT_TRUE(game.GetDirection() == Right);
}

TEST_F(SnakeGameTest, ScoreRead1) {
  remove(SCORE_FILE_SNAKE);

  Snake game;
  Controller controller (&game);
  EXPECT_TRUE(game.gameInfo.score == 0);
  remove(SCORE_FILE_SNAKE);
}

TEST_F(SnakeGameTest, ScoreRead2) {
  remove(SCORE_FILE_SNAKE);
   std::ofstream out;         
    out.open(SCORE_FILE_SNAKE);      
    if (out.is_open())
    {
        out << 156;
    }
    out.close(); 
  Snake game;
  Controller controller (&game);
  EXPECT_TRUE(game.gameInfo.high_score == 156);
  remove(SCORE_FILE_SNAKE);
}

TEST_F(SnakeGameTest, ScoreWrite) {
  remove(SCORE_FILE_SNAKE);

  Snake game;
  Controller controller (&game);
  game.gameInfo.score = 156;
  EXPECT_TRUE(game.gameInfo.score == 156);
  int scre;
  std::string line;

  std::ifstream in(SCORE_FILE_SNAKE);
    if (in.is_open())
    {
       std::getline(in, line);
        std::stringstream ss(line);
        ss >> scre;         
    }
  EXPECT_TRUE(game.gameInfo.score == 156);
}

TEST_F(SnakeGameTest, UpdateLevel){
   Snake game;
  Controller controller (&game);
  game.UpdateLevel();
  EXPECT_TRUE(game.gameInfo.level == 1);
  for (int i = 2; i <= 10; i++) {
    game.gameInfo.score += 5;
    game.UpdateLevel();
    EXPECT_EQ(game.gameInfo.level, i);
  }
   game.gameInfo.score += 5;
  game.UpdateLevel();
    EXPECT_EQ(game.gameInfo.level, 10);

}

TEST_F(SnakeGameTest, EatApple){
  remove(SCORE_FILE_SNAKE);
   Snake game;
  Controller controller (&game);
  game.gameInfo.next[0][0] = game.snakeCoordinates.front().x;
  game.gameInfo.next[0][1] = game.snakeCoordinates.front().y - 1;
  game.GameStart();
  game.MovingSnake();
  EXPECT_EQ(game.gameInfo.score, 1);
}

TEST_F(SnakeGameTest, moving1){
   Snake game;
  Controller controller (&game);
  game.GameStart();
  game.MovingSnake();
  EXPECT_EQ(game.snakeCoordinates.front().x, game.snakeCoordinates[1].x);
  EXPECT_EQ(game.snakeCoordinates.front().y, game.snakeCoordinates[1].y - 1);
}

TEST_F(SnakeGameTest, moving2){
   Snake game;
  Controller controller (&game);
  game.GameStart();

  controller.userInput(Right, 0);
  game.MovingSnake();
  EXPECT_EQ(game.snakeCoordinates.front().x, game.snakeCoordinates[1].x + 1);
  EXPECT_EQ(game.snakeCoordinates.front().y, game.snakeCoordinates[1].y);
}

TEST_F(SnakeGameTest, moving3){
   Snake game;
  Controller controller (&game);
  game.GameStart();
  controller.userInput(Left, 0);
  game.MovingSnake();
  EXPECT_EQ(game.snakeCoordinates.front().x, game.snakeCoordinates[1].x -1);
  EXPECT_EQ(game.snakeCoordinates.front().y, game.snakeCoordinates[1].y);
}

TEST_F(SnakeGameTest, moving4){
   Snake game;
  Controller controller (&game);
  game.GameStart();
controller.userInput(Left, 0);
  game.MovingSnake();

controller.userInput(Down, 0);
  game.MovingSnake();
  EXPECT_EQ(game.snakeCoordinates.front().x, game.snakeCoordinates[1].x );
  EXPECT_EQ(game.snakeCoordinates.front().y, game.snakeCoordinates[1].y + 1);
}

TEST_F(SnakeGameTest, moving5){
   Snake game;
  Controller controller (&game);
  game.GameStart();
controller.userInput(Action, 0);
  game.MovingSnake();

  EXPECT_EQ(game.snakeCoordinates.front().x, game.snakeCoordinates[1].x );
  EXPECT_EQ(game.snakeCoordinates.front().y, game.snakeCoordinates[1].y - 1);
}

TEST_F(SnakeGameTest, updateState){
   Snake game;
  Controller controller (&game);
  game.GameStart();
  game.gameInfo.speed = -10000000;
    EXPECT_NO_THROW(controller.updateCurrentState());
}

TEST_F(SnakeGameTest, GetFlagError){
  remove(SCORE_FILE_SNAKE);
   Snake game;
  Controller controller (&game);
  EXPECT_EQ( game.GetFlagErrorGame(), false);
  remove(SCORE_FILE_SNAKE);

}


} // namespace s21



