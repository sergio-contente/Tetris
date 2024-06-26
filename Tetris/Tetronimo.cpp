#include "Tetronimo.h"
#include "Constants.h"

/*
 * Block Shape 4 x 4
 *  [ 0   1     2   3]
 *  [ 4   5     6   7]
 *  [ 8   9     10  11]
 *  [ 12  13    14  15]
 * */

 // 4 piece
unsigned int BlockInfo4x4[7][4] = {

        {4, 5,  8,  9},    // Square => id 0
        {5, 9, 13,  12},    // Reverse-L => id 1
        {5, 9, 10,  14},    // Z => id 2
        {5, 9,  8,  12},    // Reverse-Z => id 3
        {5, 8,  9,  10},    // T => id 4
        {5, 9,  13, 14},    // L => id 5
        {1, 5,  9,  13},    // I => id 6
};

Tetromino::Tetromino(sf::Texture& texture, int id)
    : mPosition(sf::Vector2i{ 3, 0 }), mCurrentRotation(0), mID(id), mBlock(), mSprite(texture, sf::IntRect{ (id % 7) * 18, 0, 18, 18 }) {
    mID = mID % 7; //restrict so it does not get out of bounce
    for (auto i = 0; i < 4; ++i) {
        mBlock[i].x = BlockInfo4x4[mID][i] % 4;
        mBlock[i].y = BlockInfo4x4[mID][i] / 4;
    }
}

void Tetromino::rotate() {
    //store state of Block in case rotation turns out to be invalid
    mOldBlock = mBlock;

    if (mID == 0) { //square: no need for rotation
        return;
    }
    if (mID == 6) { // I: restrict "rotation" to two states (horizontal/vertical)
        mCurrentRotation++;
        for (auto i = 0; i < 4; ++i) {
            sf::Vector2i oldPoint = mBlock[i];    //pivot
            sf::Vector2i localVector = oldPoint - sf::Vector2i{ 1, 2 };
            sf::Vector2i nextPoint{};
            if (mCurrentRotation % 2 == 1) {
                /* counter-clockwise
                 * [0  -1]
                 * [-1  0]*/
                nextPoint = sf::Vector2i{ (0 * localVector.x) + (-1 * localVector.y),
                                         (1 * localVector.x) + (0 * localVector.y) };

            }
            else {

                nextPoint = sf::Vector2i{ (0 * localVector.x) + (1 * localVector.y),
                                         (-1 * localVector.x) + (0 * localVector.y) };

            }
            mBlock[i] = sf::Vector2i{ 1,2 } + nextPoint;

        }
        return;
    }
    for (auto i = 0; i < 4; ++i) {
        sf::Vector2i oldPoint = mBlock[i];    //pivot
        sf::Vector2i localVector = oldPoint - sf::Vector2i{ 1,2 };   // 1, 1

        /*//Rotation Matrix
         * [cos Degree    -sin Degree]
         * [sin Degree     cos Degree]
         * translates to
         * clockwise
         * [0   -1]
         * [1    0]
         * */

        sf::Vector2i nextPoint{ (0 * localVector.x) + (-1 * localVector.y),
                                (1 * localVector.x) + (0 * localVector.y) };
        mBlock[i] = sf::Vector2i{ 1,2 } + nextPoint;
    }
}

void Tetromino::move(Direction dir) {
    if (dir == Direction::Left) {
        mPosition.x--;
    }
    else if (dir == Direction::Right) {
        mPosition.x++;
    }
    else {
        mPosition.y++;
    }
}

void Tetromino::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    float x_offset = (- 18.f * scale * 10 / 2 + real_widht / 2) - block_size * 5;
    float y_offset = (-18.f * scale * 18 / 2 + real_height / 2);
    for (int i = 0; i < 4; ++i) {
        mSprite.setPosition((mBlock[i].x * 18 * scale) + (mPosition.x * 18 * scale) + x_offset, (mBlock[i].y * 18 * scale) + (mPosition.y * 18 * scale) + y_offset);
        mSprite.setScale(scale, scale); // Scales the sprite by 2 times in both x and y directions
        target.draw(mSprite);
    }
}

std::array<sf::Vector2i, 4> Tetromino::getBlockPositions() const {
    std::array<sf::Vector2i, 4> blockPositions;
    for (auto i = 0; i < 4; ++i) {
        blockPositions[i] = sf::Vector2i{ mBlock[i].x + mPosition.x, mBlock[i].y + mPosition.y };
    }
    return blockPositions;
}

std::array<sf::Vector2i, 4> Tetromino::getFutureBlockPositions(Direction direction) const {

    std::array<sf::Vector2i, 4> blockPositions;
    sf::Vector2i tempPosition{ mPosition };
    if (direction == Direction::Left) {
        tempPosition.x--;
    }
    else if (direction == Direction::Right) {
        tempPosition.x++;
    }
    else {
        tempPosition.y++;
    }
    for (auto i = 0; i < 4; ++i) {
        blockPositions[i] = sf::Vector2i{ mBlock[i].x + tempPosition.x, mBlock[i].y + tempPosition.y };
    }
    return blockPositions;
}

void Tetromino::revertState() {
    mBlock = mOldBlock;
}

int Tetromino::getID() const {
    return mID;
}

void Tetromino::setPosition(const sf::Vector2i& position) {
    mPosition = position;
}
