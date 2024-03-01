#pragma once

#include <array>
#include <SFML/Graphics.hpp>

class Tetromino {
private:
    std::array<std::array<int, 4>, 4> shape; // 4x4 grid for the tetromino shape
    sf::Vector2i position; // Position on the game board
    sf::Color color; // Color of the tetromino

public:
    // Constructor
    Tetromino(const std::array<std::array<int, 4>, 4>& shape, sf::Color color)
        : shape(shape), color(color), position(sf::Vector2i(5, 0)) {} // Start position can vary based on game design

    // Accessors
    const std::array<std::array<int, 4>, 4>& getShape() const { return shape; }
    const sf::Vector2i& getPosition() const { return position; }
    const sf::Color& getColor() const { return color; }

    // Movement methods
    void moveLeft() { position.x -= 1; }
    void moveRight() { position.x += 1; }
    void moveDown() { position.y += 1; }

    // Rotate the tetromino (clockwise for simplicity)
    void rotate() {
        std::array<std::array<int, 4>, 4> rotatedShape;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                rotatedShape[j][3 - i] = shape[i][j];
            }
        }
        shape = rotatedShape;
    }
};
