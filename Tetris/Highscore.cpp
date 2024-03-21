#include "Highscore.h"
#include "Constants.h"

Highscore::Highscore()
    : mFont(), mLevelText(), mScoreText(), mClearedLinesText(), mScore(0), mLoadSoftScore(0),
    mLinesCleared(0) {
    mFont.loadFromFile("assets/Computerfont.ttf");
    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(real_height * 0.023 * scale);

    mLevelText.setFont(mFont);
    mLevelText.setCharacterSize(real_height * 0.023 * scale);
    mScoreText.setPosition(sf::Vector2f{ (x_blocs * block_size * scale + real_widht / 2) - block_size * scale * 6, real_height / 2 * 0.65 - block_size * 6});
    mLevelText.setPosition(sf::Vector2f{ (x_blocs * block_size * scale + real_widht / 2) - block_size * scale * 6, real_height / 2 * 0.85 - block_size * 6});
    mClearedLinesText.setFont(mFont);
    mClearedLinesText.setCharacterSize(real_height * 0.023 * scale);
    mClearedLinesText.setPosition((x_blocs * block_size * scale + real_widht / 2) - block_size * scale * 6, real_height / 2 * 1.05 - block_size * 6);
}

void Highscore::update(const sf::Time& dt) {
    mLevelText.setString(std::string{ "Level:\n" + std::to_string(mLinesCleared / 10) });
    mScoreText.setString(std::string{ "Score:\n" + std::to_string(mScore) });
    mClearedLinesText.setString(std::string{ "Lines:\n" + std::to_string(mLinesCleared) });
}

void Highscore::reset() {
    mLinesCleared = 0;
    mScore = 0;
}

void Highscore::addSoftScore(int score) {
    mLoadSoftScore += score;
}

void Highscore::sumSoftScore() {
    mScore += mLoadSoftScore;
    mLoadSoftScore = 0;
}

void Highscore::draw(sf::RenderWindow& window) {
    window.draw(mLevelText);
    window.draw(mScoreText);
    window.draw(mClearedLinesText);
}

void Highscore::addClearedLines(int num) {
    mLinesCleared += num;
    int level = mLinesCleared / 10;
    switch (num) {
    case (1): {
        mScore += 40 * (level + 1);
        break;
    }
    case (2): {
        mScore += 100 * (level + 1);
        break;
    }
    case (3): {
        mScore += 300 * (level + 1);
        break;
    }
    case (4): {
        mScore += 1200 * (level + 1);
        break;
    }
    }
}

int Highscore::getLevel() const {
    return mLinesCleared / 10;
}
int Highscore::getScore() {
    return this->mScore;
}