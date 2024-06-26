#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H

//#define NOMINMAX
//#include <windows.h>

#include <SFML/Graphics.hpp>

#include "State.h"
#include "Game.h"
#include "Tetronimo.h"
#include "Board.h"
#include "Highscore.h"
#include "Utils.h"
#include "Direction.h"
#include "GameOverState.h"

class Board;

class GamePlayState : public State
{
private:
	std::shared_ptr <Context> m_context;
    std::shared_ptr <NetworkManager> m_networkManager;
    std::unique_ptr <Tetromino> m_tetromino;
    std::unique_ptr <Tetromino >m_preview;
    std::unique_ptr <Board> m_board;
    sf::Sprite m_backgroundSprite;
    sf::Sprite m_gameHud;
    sf::Time mElapsedTime;
    int mID;
public:
    Highscore m_HighScore;
    int lastHighScore;
    sf::Texture   mTexture;
    GamePlayState(std::shared_ptr <Context>& context, std::shared_ptr <NetworkManager>& m_networkManager);
	~GamePlayState();

    virtual void Init() override;
    virtual void ProcessInput() override;
    virtual void Update(const sf::Time& deltaTime) override;
    virtual void Draw() override;

    virtual void Pause() override;
    virtual void Start() override;

    void createTetromino();
    void proceed(Direction dir);
    void rotate();
    bool isValidMovement(std::array<sf::Vector2i, 4> block);
    bool isOccupied(int x, int y);
};

#endif
