#include "tetromino.hpp"
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <unistd.h>
#include <ncurses.h>

class TetrisGame {
    public:
    TetrisGame();
    ~TetrisGame();
    void run();

    private:
    std::vector<std::vector<int>> board;
    void processInput();
    void update();
    void render();
    void gameOver();
    void spawnPiece();
    bool isValidMove(int x, int y, const std::vector<std::vector<int>>& piece);
    void mergePiece(int x, int y, const std::vector<std::vector<int>>& piece);
    void clearLines();
    void rotatePiece();
    void setCursorPosition(int x, int y);
    void setColor(int color);

    int currentX, currentY;
    std::vector<std::vector<int>> currentPiece;
    int linesCleared;
    bool isGameOver;
};
