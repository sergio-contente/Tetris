#include "GamePlayState.h"

float Xvariation = -1;
float Yvariation = -1;

GamePlayState::GamePlayState(std::shared_ptr <Context>& context, std::shared_ptr <NetworkManager>& m_networkManager) :
    m_context(context), m_networkManager(m_networkManager), m_tetromino(nullptr), m_preview(nullptr),
    m_HighScore(), mElapsedTime(sf::Time::Zero), mID(getRandomNumber(7))
{
    m_context->m_assets->LoadTexture("Blocks", "assets/tetris-texture.png");
    mTexture = m_context->m_assets->GetTexture("Blocks");
    this->m_board = std::make_unique<Board>(sf::Vector2i{ 10, 18 }, *this);
    createTetromino();
}

GamePlayState::~GamePlayState() {

}

void GamePlayState::Init() {
    // get the game play hud
    m_context->m_assets->LoadTexture("GameHUD", "assets/Game-hud.png");

    // Load the background Texture
    m_context->m_assets->LoadTexture("Background", "assets/Background.png");

    // Get the texture and set it to be repeated
    sf::Texture& backgroundTexture = m_context->m_assets->GetTexture("Background");
    backgroundTexture.setRepeated(true);

    // Create the background sprite
    m_backgroundSprite.setTexture(backgroundTexture);
    // Set the texture rect to cover the whole screen
    m_backgroundSprite.setTextureRect(sf::IntRect(0, 0, m_context->m_window->getSize().x, m_context->m_window->getSize().y));

    // Load the game hud texture
    m_gameHud.setTexture(m_context->m_assets->GetTexture("GameHUD"));
}

void GamePlayState::createTetromino() {
    m_tetromino.reset(new Tetromino{ mTexture, mID });
    // create new game if necessary
    if (m_board->isOccupied(m_tetromino->getBlockPositions())) {
        m_board->clean();
        this->lastHighScore = m_HighScore.getScore();
        m_context->m_states->Add(std::make_unique<GameOverState>(m_context, m_networkManager, this->lastHighScore), false);
        m_HighScore.reset();
        Xvariation = -1;
        Yvariation = -1;
    }
    mID = getRandomNumber(7);
    m_preview.reset(new Tetromino{ mTexture, mID });
    m_preview->setPosition(sf::Vector2i{ 12, 13 });
}


void GamePlayState::rotate() {
    if (!m_tetromino) return;
    m_tetromino->rotate();
    if (!isValidMovement(m_tetromino->getBlockPositions())) {
        m_tetromino->revertState();
    }
}


void GamePlayState::ProcessInput() {
    sf::Event e;
    while (m_context->m_window->pollEvent(e)) {
        if (e.type == sf::Event::Closed) m_context->m_window->close();
        else if (e.type == sf::Event::KeyPressed) {
            if (e.key.code == sf::Keyboard::S) {
                if (Xvariation == 0) {
                    Yvariation = Yvariation - 2;
                }
                else {
                Xvariation = 0;
                Yvariation = -2;
                }
                proceed(Direction::SoftDown);
            }
            else if (e.key.code == sf::Keyboard::D) {
                if (Xvariation >= 1 && Yvariation >= 1) {
                    Xvariation += 1;
                    Yvariation += 1;
                }
                else {
                    Xvariation = 1;
                    Yvariation = 1;
                }
                proceed(Direction::Right);
            }
            else if (e.key.code == sf::Keyboard::A) {
                if (Xvariation <= -1 && Yvariation >= 1) {
                    Xvariation += -1;
                    Yvariation += 1;
                }
                else {
                    Xvariation = -1;
                    Yvariation = 1;
                }
                proceed(Direction::Left);
            }
            else if (e.key.code == sf::Keyboard::Space) {
                if (Xvariation == 0) {
                    Xvariation = 1;
                }
                if (Yvariation == 0) {
					Yvariation = 1;
				}
                else if (Xvariation >= 1 && Yvariation >= 1) {
                    Xvariation = -1;
                    Yvariation = 1;
                }
                else if (Xvariation <= -1 && Yvariation >= 1) {
                    Xvariation = -1;
					Yvariation = -1;
				}
                else if (Xvariation <= -1 && Yvariation <= -1) {
					Xvariation = 1;
                    Yvariation = -1;
				}
                else if (Xvariation >= 1 && Yvariation <= -1) {
                    Xvariation = 1;
                    Yvariation = 1;
                }
                rotate();
            }
            else if (e.key.code == sf::Keyboard::P) {
                m_board->printBoard();
            }
            else if (e.key.code == sf::Keyboard::I) {
                m_HighScore.addClearedLines(10);
            }
        }
    }
}


bool GamePlayState::isValidMovement(std::array<sf::Vector2i, 4> block) {
    for (int i = 0; i < 4; ++i) {
        if (block[i].x < 0 || block[i].x > 9 || block[i].y > 17) {
            return false;
        }
        if (isOccupied(block[i].x, block[i].y)) {
            return false;
        }
    }
    return true;
}

bool GamePlayState::isOccupied(int x, int y) {
    return m_board->getField(x, y)->mOccupied;
}


void GamePlayState::Update(const sf::Time& deltaTime) {
    sf::Clock clock;
    sf::Time trigger{ sf::seconds(85.f / (85.f + (m_HighScore.getLevel() * (m_HighScore.getLevel() * 5.f)))) };
    mElapsedTime += deltaTime;
    if (mElapsedTime > trigger) {
        mElapsedTime = sf::Time::Zero;
        proceed(Direction::Down);
    }
    m_board->update(deltaTime);
    m_HighScore.update(deltaTime);
    if (!m_tetromino) {
        if (m_board->isToRemoveBlocks()) {
            return;
        }
        createTetromino();
    }
    // Add code to move the background
    static float backgroundX = 0.f; // These static variables will keep their values between updates
    static float backgroundY = 0.f;
    const float backgroundSpeedX = 100.f; // Speed of the background movement along X-axis
    const float backgroundSpeedY = 50.f;  // Speed of the background movement along Y-axis

    // Calculate new positions based on deltaTime (might get an overflow if the game runs for a long time)
    backgroundX += backgroundSpeedX * deltaTime.asSeconds() * Xvariation;
    backgroundY += backgroundSpeedY * deltaTime.asSeconds() * Yvariation;

    m_backgroundSprite.setTextureRect(sf::IntRect(static_cast<int>(backgroundX), static_cast<int>(backgroundY), m_context->m_window->getSize().x, m_context->m_window->getSize().y));

}
void GamePlayState::Draw() {
    m_context->m_window->clear(sf::Color::Blue);
    m_context->m_window->draw(m_backgroundSprite); // Draw the background sprite first
    m_context->m_window->draw(m_gameHud);
    m_HighScore.draw(*m_context->m_window);
    m_board->draw(*m_context->m_window);
    if (m_tetromino) {
        m_context->m_window->draw(*m_tetromino);
    }
    m_context->m_window->draw(*m_preview);
    m_context->m_window->display();
}

void GamePlayState::Pause() {

}
void GamePlayState::Start() {

}

void GamePlayState::proceed(Direction dir) {
    if (!m_tetromino) return;

    if (isValidMovement(m_tetromino->getFutureBlockPositions(dir))) {
        m_tetromino->move(dir);
        if (dir == Direction::SoftDown) m_HighScore.addSoftScore(1);
    }
    else {
        if (dir == Direction::Down || dir == Direction::SoftDown) {
            int id = m_tetromino->getID();
            m_board->addBlock(id, m_tetromino->getBlockPositions());
            m_tetromino.reset(nullptr);
            m_HighScore.sumSoftScore();
        }
    }
}
