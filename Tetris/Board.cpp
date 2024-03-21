#include "Board.h"
#include "Constants.h"

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

Board::Board(sf::Vector2i size, GamePlayState& game, std::shared_ptr <NetworkManager>& m_networkManager)
    : mGame(game), mFields(), mFieldInfos(), mSize(size), mYCleaned(), mElapsedTime(0.f), mToRemoveBlocks(false), m_networkManager(m_networkManager) {
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            mFields[convert2D_to_1D(x, y)] = std::make_unique<Field>();
        }
    }
    for (int id = 0; id < 7; ++id) {
        mFieldInfos[id] = std::make_unique<FieldInfo>(mGame.mTexture, id);
    }
}

void Board::addBlock(int id, const std::array<sf::Vector2i, 4>& positions) {
    for (const auto& position : positions) {
        Field* field = getField(position.x, position.y);
        field->mOccupied = true;
        field->mInfo = mFieldInfos[id].get();
    }
}

int Board::convert2D_to_1D(int x, int y) {
    return y * mSize.x + x;
}

void Board::draw(sf::RenderWindow& window) {
    float x_offset = (-block_size * scale * mSize.x / 2 + real_widht / 2) - block_size * 5;
    float y_offset = - block_size * scale * mSize.y / 2+ real_height/2;
    for (int x = 0; x < mSize.x; ++x) {
        for (int y = 0; y < mSize.y; ++y) {

            auto field = getField(x, y);
            if (field->mOccupied && field->mVisible) {
                field->mInfo->mSprite.setPosition(x * 18.f * scale + x_offset, y * 18.f * scale + y_offset);
                field->mInfo->mSprite.setScale(scale, scale);
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

    int countClearedLines = 0;
    for (int y = mSize.y - 1; y >= 0; --y) {
        bool isLineComplete = true;
        for (int x = 0; x < mSize.x; ++x) {
            if (!getField(x, y)->mOccupied) {
                isLineComplete = false;
                break;
            }
        }
        if (isLineComplete) {
            mYCleaned.push_back(y);
            mToRemoveBlocks = true;
            countClearedLines++;
        }
    }

    mGame.m_HighScore.addClearedLines(countClearedLines);
    if (countClearedLines > 0) {
        m_networkManager->notifyAttack(countClearedLines);
    }
}



void Board::cleanLines() {
    if (mYCleaned.empty()) return;
    std::cout << "Limpando as linhas" << std::endl;
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
    if (m_networkManager->getLinesAdded() > 0) {
        std::cout << "LINHAS ADICIONADAS " << m_networkManager->getLinesAdded() << std::endl;
        addAttackRows(m_networkManager->getLinesAdded());
        m_networkManager->resetLinesAdded();
        std::cout << "LINHAS RESETADAS: " << m_networkManager->getLinesAdded() << std::endl;
    }
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

int Board::getBlocksFromEnemy() {
    return countBlocks;
}

void Board::addAttackRows(int linestoAdd) {
    // Deslocando as linhas existentes para cima.
    for (int y = 0; y < mSize.y - linestoAdd; ++y) {
        for (int x = 0; x < mSize.x; ++x) {
            *getField(x, y) = *getField(x, y + linestoAdd);
        }
    }

    // Adicionando novas linhas de ataque com lacunas aleatórias.
    for (int i = 0; i < linestoAdd; ++i) {
        int newY = mSize.y - 1 - i;
        int gapPosition = rand() % mSize.x; // Posição aleatória para a lacuna na linha.
        for (int x = 0; x < mSize.x; ++x) {
            if (x != gapPosition) { // Se não for a posição da lacuna, adiciona o bloco.
                addBlock(0, { sf::Vector2i(x, newY) }); // Supõe-se que 'addBlock' aceite o ID do bloco e uma única posição.
            }
        }
    }
}

bool Board::isOccupiedGap(std::array<sf::Vector2i, 4> block, int x) {
    for (int i = 0; i < 4; ++i) {
        auto  field = getField(block[i].x, block[i].y);
        if ((field->mOccupied) && (block[i].x == x)) {
            return true;
        }
    }
    return false;
}