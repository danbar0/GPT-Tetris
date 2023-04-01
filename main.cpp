#include "TetrisGame.hpp"

int main() {
  // Seed the random number generator
  srand(static_cast<unsigned int>(time(nullptr)));

  // Create and run the Tetris game
  TetrisGame game;
  game.run();

  return 0;
}
