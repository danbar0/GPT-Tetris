#include "TetrisGame.hpp"

// TetrisGame constructor
TetrisGame::TetrisGame() {
    // Initialize the game board with 0s (empty spaces)
    board.resize(BOARD_HEIGHT, std::vector<int>(BOARD_WIDTH, 0));

    // Spawn the first piece
    spawnPiece();

    // Set the initial position of the piece (top center of the board)
    currentX = BOARD_WIDTH / 2 - currentPiece[0].size() / 2;
    currentY = 0;

    // Initialize the lines cleared counter
    linesCleared = 0;

    // Set the initial game state
    isGameOver = false;
}

// Main game loop
void TetrisGame::run() {
    using namespace std::chrono;
    steady_clock::time_point currentTime = steady_clock::now();
    steady_clock::time_point lastUpdateTime = steady_clock::now();
    duration<double> timeStep(0.5);  // Controls the falling speed of the pieces

    while (!isGameOver) {
        // Process user input
        processInput();

        // Update the game state at regular intervals based on the timeStep
        currentTime = steady_clock::now();
        if (duration_cast<duration<double>>(currentTime - lastUpdateTime) >= timeStep) {
            update();
            lastUpdateTime = currentTime;
        }

        // Render the game
        render();

        // Sleep for a short duration to avoid consuming 100% CPU
        Sleep(20);
    }

    // Display the game over message
    gameOver();
}

// Process user input
void TetrisGame::processInput() {
  if (_kbhit()) {
    int key = _getch();
    int newX = currentX;
    int newY = currentY;

    switch (key) {
      case 224:  // Arrow keys have a 224 prefix
        key = _getch();  // Get the actual arrow key code
        switch (key) {
          case 75:  // Left arrow key
            newX--;
            break;
          case 77:  // Right arrow key
            newX++;
            break;
          case 72:  // Up arrow key (rotate)
            rotatePiece(currentPiece);
            break;
          case 80:  // Down arrow key (fast drop)
            while (isValidMove(currentX, currentY + 1, currentPiece)) {
              currentY++;
            }
            break;
        }
        break;
    }

    // Check if the move is valid and update the position if it is
    if (isValidMove(newX, newY, currentPiece)) {
      currentX = newX;
      currentY = newY;
    }
  }
}


// Update game state
void TetrisGame::update() {
  // Move the current piece down by one unit
  int newX = currentX;
  int newY = currentY + 1;

  // Check if the move is valid
  if (isValidMove(newX, newY, currentPiece)) {
    // Update the piece's position
    currentY = newY;
  } else {
    // Merge the current piece into the game board
    mergePiece(currentX, currentY, currentPiece);

    // Clear any completed lines
    clearLines();

    // Spawn a new piece
    spawnPiece();

    // Set the initial position of the new piece (top center of the board)
    currentX = BOARD_WIDTH / 2 - currentPiece[0].size() / 2;
    currentY = 0;

    // Check if the new piece can be placed on the board
    if (!isValidMove(currentX, currentY, currentPiece)) {
      // If not, set the game over flag
      isGameOver = true;
    }
  }
}


// Render the game
void TetrisGame::render() {
  // Draw the game board, pieces, and score
}

// Handle game over
void TetrisGame::gameOver() {
  // Display a game over message and stop the game loop
}

// Spawn a new piece
void TetrisGame::spawnPiece() {
  // Select a random Tetromino and set its starting position
}

// Check if a move is valid
bool TetrisGame::isValidMove(int x, int y, const std::vector<std::vector<int>>& piece) {
// Check for collisions with the board boundaries and other pieces
}

// Merge the current piece with the game board
void TetrisGame::mergePiece(int x, int y, const std::vector<std::vector<int>>& piece) {
// Add the current piece to the board
}

// Clear completed lines
void TetrisGame::clearLines() {
// Check for full lines, remove them, and update the score
}

// Rotate the current piece
void TetrisGame::rotatePiece(std::vector<std::vector<int>>& piece) {
// Perform a 90-degree clockwise rotation
}

// Set the console cursor position
void TetrisGame::setCursorPosition(int x, int y) {
// Set the position of the console cursor
}

// Set the console text color
void TetrisGame::setColor(int color) {
// Set the color of the console text
}
