#pragma once

#include <array>
#include <SFML/Graphics.hpp>

class Tetromino : public sf::Drawable {
public:
    Tetromino(sf::Texture& texture, int id);
    ~Tetromino() = default;
    void rotate();

    void move(Direction dir);
    void setPosition(const sf::Vector2i& position);
    void revertState();
    std::array<sf::Vector2i, 4> getBlockPositions() const;
    std::array<sf::Vector2i, 4> getFutureBlockPositions(Direction direction) const;
    int getID() const;
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::Vector2i                    mPosition;
    int                             mCurrentRotation;
    int                             mID;
    std::array<sf::Vector2i, 4>     mBlock;
    std::array<sf::Vector2i, 4>     mOldBlock;
    mutable sf::Sprite              mSprite;
};
