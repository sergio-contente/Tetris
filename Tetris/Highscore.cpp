#include "Highscore.h"

Highscore::Highscore()
    : mFont(), mLevelText(), mScoreText(), mClearedLinesText(), mScore(0), mLoadSoftScore(0),
    mLinesCleared(0) {
    mFont.loadFromFile("Dong.ttf");
    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(15);

    mLevelText.setFont(mFont);
    mLevelText.setCharacterSize(15);
    mScoreText.setPosition(sf::Vector2f{ 10 * 18 + 3, 50.f });
    mLevelText.setPosition(sf::Vector2f{ 10 * 18 + 3, 100.f });

    mClearedLinesText.setFont(mFont);
    mClearedLinesText.setCharacterSize(15);
    mClearedLinesText.setPosition(10 * 18 + 3, 150.f);
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
