
#ifndef BOARD_H
#define BOARD_H


#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <array>
#include <unordered_map>

class Game;
struct FieldInfo {
    FieldInfo(sf::Texture& texture, int id);
    sf::Sprite          mSprite;


};
struct Field {

    Field& operator=(const Field& field);
    bool mOccupied = false;
    bool mVisible = true;
    FieldInfo* mInfo = nullptr;
};


class Board {
public:
    Board(sf::Vector2i size, Game& game);
    Board(const Board& other) = delete;
    Board& operator= (const Board& other) = delete;

    void update(const sf::Time& dt);
    void clean();
    void addBlock(int id, std::array<sf::Vector2i, 4> block);
    bool isOccupied(std::array<sf::Vector2i, 4> block);
    void draw(sf::RenderWindow& window);
    void printBoard();
    inline bool isToRemoveBlocks() const { return mToRemoveBlocks; }
    Field* getField(int x, int y);
private:
    int convert2D_to_1D(int x, int y);
    void cleanLines();
    void markLinesForRemoval();
    void blink();

    Game& mGame;
    std::unordered_map<unsigned int, std::unique_ptr<Field>>        mFields;
    std::unordered_map<unsigned int, std::unique_ptr<FieldInfo>>    mFieldInfos;
    sf::Vector2i                                                    mSize;
    std::vector<int>                                                mYCleaned;
    float                                                           mElapsedTime;
    bool                                                            mToRemoveBlocks;


};


#endif //BOARD_H
