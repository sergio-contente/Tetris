#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
class Highscore {
public:
    Highscore();
    Highscore(const Highscore& other) = delete;
    Highscore& operator = (const Highscore& other) = delete;
    void draw(sf::RenderWindow& window);
    void reset();
    void addSoftScore(int score);
    void sumSoftScore();
    void addClearedLines(int num);
    void update(const sf::Time& dt);
    int getLevel() const;
    int getScore();
private:
    sf::Font            mFont;
    sf::Text            mLevelText;
    sf::Text            mScoreText;
    sf::Text            mClearedLinesText;
    int                 mScore;
    int                 mLoadSoftScore;
    int                 mLinesCleared;


};


