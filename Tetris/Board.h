#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "Tetronimo.h"

class Board {
private:
    const int width = 10;
    const int height = 20;
    std::vector<std::vector<sf::Color>> grid; // 2D grid representing the board

public:
    Board() : grid(height, std::vector<sf::Color>(width, sf::Color::Black)) {}

    // Accessor for the grid for drawing purposes
    const std::vector<std::vector<sf::Color>>& getGrid() const {
        return grid;
    }

    // Check if a move or rotation is possible
    bool isMovePossible(const Tetromino& tetromino, sf::Vector2i direction) const {
        auto shape = tetromino.getShape();
        auto pos = tetromino.getPosition() + direction;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (shape[i][j]) {
                    int newX = pos.x + j;
                    int newY = pos.y + i;

                    // Check boundaries
                    if (newX < 0 || newX >= width || newY < 0 || newY >= height) return false;

                    // Check if the position is already filled
                    if (grid[newY][newX] != sf::Color::Black) return false;
                }
            }
        }
        return true;
    }

    // Add a tetromino to the board
    void lockTetromino(const Tetromino& tetromino) {
        auto shape = tetromino.getShape();
        auto pos = tetromino.getPosition();
        auto color = tetromino.getColor();

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (shape[i][j]) {
                    int newX = pos.x + j;
                    int newY = pos.y + i;

                    // Fill the cell with the tetromino's color
                    grid[newY][newX] = color;
                }
            }
        }
    }

    // Clear completed lines
    void clearLines() {
        for (int row = 0; row < height; ++row) {
            bool isLineComplete = true;
            for (int col = 0; col < width; ++col) {
                if (grid[row][col] == sf::Color::Black) {
                    isLineComplete = false;
                    break;
                }
            }

            if (isLineComplete) {
                for (int clearRow = row; clearRow > 0; --clearRow) {
                    for (int col = 0; col < width; ++col) {
                        grid[clearRow][col] = grid[clearRow - 1][col];
                    }
                }

                // Clear the top row
                for (int col = 0; col < width; ++col) {
                    grid[0][col] = sf::Color::Black;
                }
            }
        }
    }
};


