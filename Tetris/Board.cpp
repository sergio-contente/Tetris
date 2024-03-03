#include "Board.h"
#include "Game.h"
#include "Utils.h"
#include "Highscore.h"

#include <iostream>
#include <algorithm>
#include <fstream>

FieldInfo::FieldInfo(sf::Texture& texture, int id) {
    sf::IntRect rect{ (id % 7) * 18, 0, 18,18 };
    mSprite.setTexture(texture);
    mSprite.setTextureRect(rect);
}

Field& Field::operator=(const Field& field) {
    this->mOccupied = field.mOccupied;
    this->mVisible = field.mVisible;
    this->mInfo = field.mInfo;
    return *this;
}

Board::Board(sf::Vector2i size, Game& game)
    : mGame(game), mFields(), mFieldInfos(), mSize(size), mYCleaned(), mElapsedTime(0.f), mToRemoveBlocks(false) {
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            mFields[convert2D_to_1D(x, y)] = std::make_unique<Field>();
        }
    }
    for (int id = 0; id < 7; ++id) {
        mFieldInfos[id] = std::make_unique<FieldInfo>(mGame.mTexture, id);
    }
}

void Board::addBlock(int id, std::array<sf::Vector2i, 4> block) {
    for (int i = 0; i < 4; ++i) {
        auto field = getField(block[i].x, block[i].y);
        field->mOccupied = true;
        field->mInfo = mFieldInfos[id].get();
    }
}
int Board::convert2D_to_1D(int x, int y) {
    return y * mSize.x + x;
}

void Board::draw(sf::RenderWindow& window) {
    for (int x = 0; x < mSize.x; ++x) {
        for (int y = 0; y < mSize.y; ++y) {

            auto field = getField(x, y);
            //if field has not been occupied yet, mInfo would be assigned to nullptr
            if (field->mOccupied && field->mVisible) {
                field->mInfo->mSprite.setPosition(x * 18.f, y * 18.f);
                window.draw(field->mInfo->mSprite);
            }
        }
    }
}

Field* Board::getField(int x, int y) {
    return mFields[convert2D_to_1D(x, y)].get();
}


void Board::clean() {
    for (int x = 0; x < mSize.x; ++x) {
        for (int y = 0; y < mSize.y; ++y) {
            auto field = getField(x, y);
            field->mOccupied = false;
            field->mVisible = true;
            field->mInfo = nullptr;
        }
    }
}

void Board::printBoard() {
    std::ofstream file;
    file.open("Log.txt");
    for (int y = 0; y < mSize.y; ++y) {
        for (int x = 0; x < mSize.x; ++x) {
            if (!getField(x, y)->mOccupied) {
                file << "O";
            }
            else {
                file << "X";
            }
        }
        file << "\n";
    }
    file.close();
}

void Board::markLinesForRemoval() {
    if (mToRemoveBlocks) return;
    int countClearedLines{ 0 };
    for (int y = mSize.y - 1; y > 0; --y) {
        int counter = 0;
        for (int x = 0; x < mSize.x; ++x) {
            auto field = getField(x, y);
            if (field->mOccupied) {
                counter++;
            }
            if (counter == 10) { // Line full
                mYCleaned.push_back(y);
                mToRemoveBlocks = true;
                countClearedLines++;
            }
        }
        counter = 0;
    }
    mGame.mHighScore.addClearedLines(countClearedLines);
    std::sort(mYCleaned.begin(), mYCleaned.end(), [](int left, int right) { return left < right; });
}

void Board::cleanLines() {
    if (mYCleaned.empty()) return;

    for (auto i : mYCleaned) {
        for (auto y = i; y >= 0; --y) {
            for (auto x = 0; x < mSize.x; ++x) {
                int up = y - 1;
                if (up < 0) continue;
                *getField(x, y) = *getField(x, up);
            }
        }
    }
    mYCleaned.clear();
    mToRemoveBlocks = false;
}

void Board::update(const sf::Time& dt) {
    markLinesForRemoval();
    if (mToRemoveBlocks) {
        mElapsedTime += dt.asSeconds();
        blink();
        if (mElapsedTime > 0.6f) {
            mElapsedTime = 0.f;
            cleanLines();
        }
    }
}

void Board::blink() {
    // speeds up blinking
    int num = int(mElapsedTime * 5.f);
    for (auto y : mYCleaned) {
        for (int x = 0; x < mSize.x; ++x) {
            getField(x, y)->mVisible = (num % 2 != 0);
        }
    }
}

bool Board::isOccupied(std::array<sf::Vector2i, 4> block) {
    for (int i = 0; i < 4; ++i) {
        auto  field = getField(block[i].x, block[i].y);
        if (field->mOccupied) {
            return true;
        }
    }
    return false;
}
