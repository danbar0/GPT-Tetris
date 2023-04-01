#include "TetrisGame.hpp"

// TetrisGame constructor
TetrisGame::TetrisGame() {
    // Initialize the ncurses library
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    timeout(0);

    // Initialize color pairs
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLUE);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);

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

TetrisGame::~TetrisGame() {
  endwin();
}

// Main game loop
void TetrisGame::run() {
    while (!isGameOver) {
        processInput();
        update();
        render();
        usleep(20000);
    }
    gameOver();
}

// Process user input
void TetrisGame::processInput() {
    int key = getch();
    int newX = currentX;
    int newY = currentY;

    switch (key) {
        case KEY_LEFT:  // Left arrow key
        newX--;
        break;
        case KEY_RIGHT:  // Right arrow key
        newX++;
        break;
        case KEY_UP:  // Up arrow key (rotate)
        rotatePiece();
        break;
        case KEY_DOWN:  // Down arrow key (fast drop)
        while (isValidMove(currentX, currentY + 1, currentPiece)) {
            currentY++;
        }
        break;
    }

    // Check if the move is valid and update the position if it is
    if (isValidMove(newX, newY, currentPiece)) {
      currentX = newX;
      currentY = newY;
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
    erase();

    for (int row = 0; row < BOARD_HEIGHT; ++row) {
        for (int col = 0; col < BOARD_WIDTH; ++col) {
            if (board[row][col] != 0) {
                attron(COLOR_PAIR(board[row][col]));
                mvprintw(row, col * 2, "[]");
                attroff(COLOR_PAIR(board[row][col]));
            }
        }
    }

    for (int row = 0; row < currentPiece.size(); ++row) {
        for (int col = 0; col < currentPiece[row].size(); ++col) {
            if (currentPiece[row][col] != 0) {
                attron(COLOR_PAIR(currentPiece[row][col]));
                mvprintw(currentY + row, (currentX + col) * 2, "[]");
                attroff(COLOR_PAIR(currentPiece[row][col]));
            }
        }
    }

    mvprintw(0, BOARD_WIDTH * 2 + 2, "Lines cleared: %d", linesCleared);

    refresh();
}



// Handle game overq
void TetrisGame::gameOver() {
  // Clear the console
  system("cls");

  // Set the console cursor position and text color
  setCursorPosition(BOARD_WIDTH, BOARD_HEIGHT / 2 - 1);
  setColor(4);  // Red text

  // Display the game over message
  std::cout << "GAME OVER";

  // Set the console cursor position and text color
  setCursorPosition(BOARD_WIDTH, BOARD_HEIGHT / 2);
  setColor(7);  // White text

  // Display the final score (lines cleared)
  std::cout << "Lines cleared: " << linesCleared;

  // Set the console cursor position
  setCursorPosition(BOARD_WIDTH, BOARD_HEIGHT / 2 + 1);

  // Prompt the player to press a key to exit the game
  std::cout << "Press any key to exit...";
  timeout(-1);
    getch();
}


// Spawn a new piece
void TetrisGame::spawnPiece() {
  // Randomly select one of the pieces
   currentPiece = TETROMINOS[rand() % TETROMINOS.size()];
}

// Check if a move is valid
bool TetrisGame::isValidMove(int newX, int newY, const std::vector<std::vector<int>>& piece) {
  for (size_t y = 0; y < piece.size(); ++y) {
    for (size_t x = 0; x < piece[y].size(); ++x) {
      // Check if the piece's cell is occupied
      if (piece[y][x]) {
        int boardX = newX + x;
        int boardY = newY + y;

        // Check if the move is out of the game board's bounds
        if (boardX < 0 || boardX >= BOARD_WIDTH || boardY < 0 || boardY >= BOARD_HEIGHT) {
          return false;
        }

        // Check if the move results in overlapping an existing block on the game board
        if (board[boardY][boardX]) {
          return false;
        }
      }
    }
  }

  return true;
}


// Merge the current piece with the game board
void TetrisGame::mergePiece(int posX, int posY, const std::vector<std::vector<int>>& piece) {
  for (size_t y = 0; y < piece.size(); ++y) {
    for (size_t x = 0; x < piece[y].size(); ++x) {
      // Check if the piece's cell is occupied
      if (piece[y][x]) {
        int boardX = posX + x;
        int boardY = posY + y;

        // Merge the occupied cell into the game board
        board[boardY][boardX] = 1;
      }
    }
  }
}

// Clear completed lines
void TetrisGame::clearLines() {
  for (int y = BOARD_HEIGHT - 1; y >= 0;) {
    bool isLineFull = true;

    // Check if the line is full
    for (int x = 0; x < BOARD_WIDTH; ++x) {
      if (!board[y][x]) {
        isLineFull = false;
        break;
      }
    }

    if (isLineFull) {
      // Clear the line and move the lines above it down
      for (int y2 = y; y2 > 0; --y2) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
          board[y2][x] = board[y2 - 1][x];
        }
      }

      // Increment the lines cleared score
      linesCleared++;
    } else {
      // Move to the next line
      y--;
    }
  }
}

// Rotate the current piece
void TetrisGame::rotatePiece() {
  std::vector<std::vector<int>> rotatedPiece = currentPiece;

  // Get the dimensions of the current piece
  size_t rows = currentPiece.size();
  size_t cols = currentPiece[0].size();

  // Resize the rotated piece
  rotatedPiece.resize(cols);
  for (size_t i = 0; i < cols; ++i) {
    rotatedPiece[i].resize(rows);
  }

  // Perform the 90-degree clockwise rotation
  for (size_t y = 0; y < rows; ++y) {
    for (size_t x = 0; x < cols; ++x) {
      rotatedPiece[x][rows - y - 1] = currentPiece[y][x];
    }
  }

  // Check if the rotated piece has a valid position on the game board
  if (isValidMove(currentX, currentY, rotatedPiece)) {
    // Apply the rotation
    currentPiece = rotatedPiece;
  }
}

// Set the console cursor position
void TetrisGame::setCursorPosition(int x, int y) {
  move(y, x * 2);
}

// Set the console text color
void TetrisGame::setColor(int color) {
  attron(COLOR_PAIR(color));
}