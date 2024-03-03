#include "GamePlayState.h"

GamePlayState::GamePlayState(std::shared_ptr <Context> &context, std::unique_ptr <Tetromino>& m_tetromino, std::unique_ptr<Tetromino>& m_preview, std::unique_ptr<Board> m_board) : m_context(context), m_tetromino(nullptr), m_preview(nullptr), 
m_board(), m_HighScore(), mElapsedTime(sf::Time::Zero), mID(getRandomNumber(7))
{
    m_context->m_assets->LoadFont("Blocks", "assets/Blocks.png");
    mTexture = m_context->m_assets->GetTexture("Blocks");
    m_board = std::make_unique<Board>(sf::Vector2i{ 10, 18 }, *this);
    createTetromino();
}

GamePlayState::~GamePlayState() {

}

void GamePlayState::Init() {

}

void GamePlayState::createTetromino() {
    m_tetromino.reset(new Tetromino{ mTexture, mID });
    // create new game if necessary
    if(m_board->isOccupied(m_tetromino->getBlockPositions())) {
        m_board->clean();
        m_HighScore.reset();
    }
    mID = getRandomNumber(7);
    m_preview.reset(new Tetromino{ mTexture, mID });
    m_preview->setPosition(sf::Vector2i{ 11, 12 });
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
                proceed(Direction::SoftDown);
            }
            else if (e.key.code == sf::Keyboard::D) {
                proceed(Direction::Right);
            }
            else if (e.key.code == sf::Keyboard::A) {
                proceed(Direction::Left);
            }
            else if (e.key.code == sf::Keyboard::Space) {
                rotate();
            }
            else if (e.key.code == sf::Keyboard::P) {
                m_board->printGrid();
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
    m_board->update(deltaTime);
    m_HighScore.update(deltaTime);
    if (!m_tetromino) {
        if (m_board->isToRemoveBlocks()) {
            return;
        }
        createTetromino();
    }

}
void GamePlayState::Draw() {
    m_context->m_window->clear(sf::Color::Blue);
    m_HighScore.draw(m_context->m_window);
    m_board->draw(m_context->m_window);

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

