#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <Windows.h>

const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;
const int WINDOW_WIDTH = BOARD_WIDTH + 4;
const int WINDOW_HEIGHT = BOARD_HEIGHT + 2;

// Define the shapes of the Tetris pieces
const std::vector<std::vector<std::vector<int>>> TETROMINOS = {
  // I
  {
    {1, 1, 1, 1}
  },
  // O
  {
    {1, 1},
    {1, 1}
  },
  // T
  {
    {0, 1, 0},
    {1, 1, 1}
  },
  // S
  {
    {0, 1, 1},
    {1, 1, 0}
  },
  // Z
  {
    {1, 1, 0},
    {0, 1, 1}
  },
  // J
  {
    {1, 0, 0},
    {1, 1, 1}
  },
  // L
  {
    {0, 0, 1},
    {1, 1, 1}
  }
};