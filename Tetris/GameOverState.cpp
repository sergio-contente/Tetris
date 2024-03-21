#include "GameOverState.h"
#include "Constants.h"

GameOverState::GameOverState(std::shared_ptr <Context>&context, std::shared_ptr <NetworkManager>& m_networkManager, int lastHighScore) :
    m_context(context),
    m_isRetryButtonSelected(true),
    m_isRetryButtonPressed(false),
    m_isMainMenuButtonSelected(false),
    m_isMainMenuButtonPressed(false),
    m_networkManager(m_networkManager),
    m_HighScore(lastHighScore)
{}

GameOverState::~GameOverState() {}

void GameOverState::Init() {
    m_context->m_assets->LoadFont("Start_Menu_Font", "assets/Computerfont.ttf");

    //Title
    m_gameTitle.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
    m_gameTitle.setString("Game Over");
    m_gameTitle.setCharacterSize(60);
    m_gameTitle.setOrigin(m_gameTitle.getLocalBounds().width / 2.f, m_gameTitle.getLocalBounds().height / 2.f);
    m_gameTitle.setPosition(m_context->m_window->getSize().x / 2.f, m_context->m_window->getSize().y / 2 - 100.f);

    //Highscore
    m_HighScoreText.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
    m_HighScoreText.setString("Highscore: " + std::to_string(m_HighScore));
    m_HighScoreText.setCharacterSize(40);
    m_HighScoreText.setOrigin(m_HighScoreText.getLocalBounds().width / 2, m_HighScoreText.getLocalBounds().height / 2);
    m_HighScoreText.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 - 50.f);

    //Try Again Button
    m_retryButton.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
    m_retryButton.setString("Try Again");
    m_retryButton.setCharacterSize(40);
    m_retryButton.setOrigin(m_retryButton.getLocalBounds().width / 2, m_retryButton.getLocalBounds().height / 2);
    m_retryButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2);

    //Main Menu Button
    m_mainMenuButton.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
    m_mainMenuButton.setString("Main Menu");
    m_mainMenuButton.setCharacterSize(40);
    m_mainMenuButton.setOrigin(m_mainMenuButton.getLocalBounds().width / 2, m_mainMenuButton.getLocalBounds().height / 2);
    m_mainMenuButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 + 50.f);
}

void GameOverState::Draw() {
    m_context->m_window->clear(sf::Color::Blue);
    m_context->m_window->draw(m_gameTitle);
    m_context->m_window->draw(m_HighScoreText);
    m_context->m_window->draw(m_retryButton);
    m_context->m_window->draw(m_mainMenuButton);
    m_context->m_window->display();
}

void GameOverState::ProcessInput() {
    sf::Event event;
    while (m_context->m_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_context->m_window->close();
        }
        else if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Up:
            case sf::Keyboard::Down:
                // Toggle between buttons
                m_isRetryButtonSelected = !m_isRetryButtonSelected;
                m_isMainMenuButtonSelected = !m_isRetryButtonSelected;
                break;
            case sf::Keyboard::Return:
                if (m_isRetryButtonSelected) {
                    m_isRetryButtonPressed = true;
                }
                else if (m_isMainMenuButtonSelected) {
                    m_isMainMenuButtonPressed = true;
                }
                break;
            default:
                break;
            }
        }
    }
}

void GameOverState::Update(const sf::Time& deltaTime) {
    if (m_isRetryButtonSelected) {
        m_retryButton.setFillColor(sf::Color::Yellow);
        m_mainMenuButton.setFillColor(sf::Color::White);
    }
    else {
        m_mainMenuButton.setFillColor(sf::Color::Yellow);
        m_retryButton.setFillColor(sf::Color::White);
    }
    if (m_isRetryButtonPressed)
    {
        m_context->m_states->PopCurrent();
    }
    else if (m_isMainMenuButtonPressed)
    {
        m_context->m_states->PopAll();
        m_context->m_states->Add(std::make_unique<StartMenuState>(m_context, m_networkManager), true);
    }
}
