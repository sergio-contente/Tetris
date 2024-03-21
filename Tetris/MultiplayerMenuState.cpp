#include "MultiplayerMenuState.h"

MultiplayerMenuState::MultiplayerMenuState(std::shared_ptr <Context>& m_context, std::shared_ptr<NetworkManager>& networkManager):
    m_context(m_context),
    m_isHostButtonSelected(true),
    m_isHostButtonPressed(false),
    m_isClientButtonSelected(false),
    m_isClientButtonPressed(false),
    m_isExitButtonPressed(false),
    m_isExitButtonSelected(false),
    m_networkManager(networkManager)
{}

MultiplayerMenuState::~MultiplayerMenuState() {}

void MultiplayerMenuState::Init() {
    m_context->m_assets->LoadFont("Start_Menu_Font", "assets/Computerfont.ttf");

    //Title
    m_gameTitle.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
    m_gameTitle.setString("Tetris");
    m_gameTitle.setOrigin(m_gameTitle.getLocalBounds().width / 2, m_gameTitle.getLocalBounds().height / 2);
    m_gameTitle.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 - 100.f);

    m_hostButton.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
    m_hostButton.setString("Host");
    m_hostButton.setOrigin(m_hostButton.getLocalBounds().width / 2, m_hostButton.getLocalBounds().height / 2);
    m_hostButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2);

    m_clientButton.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
    m_clientButton.setString("Connect");
    m_clientButton.setOrigin(m_clientButton.getLocalBounds().width / 2, m_clientButton.getLocalBounds().height / 2);
    m_clientButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 + 50.f);

    // Inicialização do botão Exit
    m_exitButton.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
    m_exitButton.setString("Back");
    m_exitButton.setOrigin(m_exitButton.getLocalBounds().width / 2, m_exitButton.getLocalBounds().height / 2);
    m_exitButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 + 100.f); // Ajuste a posição conforme necessário

}

void MultiplayerMenuState::Draw() {
    m_context->m_window->clear(sf::Color::Blue);
    m_context->m_window->draw(m_gameTitle);
    m_context->m_window->draw(m_hostButton);
    m_context->m_window->draw(m_clientButton);
    m_context->m_window->draw(m_exitButton);
    m_context->m_window->display();
}

void MultiplayerMenuState::ProcessInput() {
    sf::Event event;
    while (m_context->m_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_context->m_window->close();
        }
        else if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Up:
                if (m_isClientButtonSelected) {
                    m_isClientButtonSelected = false;
                    m_isHostButtonSelected = true;
                }
                else if (m_isExitButtonSelected) {
                    m_isExitButtonSelected = false;
                    m_isClientButtonSelected = true;
                }
                break;
            case sf::Keyboard::Down:
                if (m_isHostButtonSelected) {
                    m_isHostButtonSelected = false;
                    m_isClientButtonSelected = true;
                }
                else if (m_isClientButtonSelected) {
                    m_isClientButtonSelected = false;
                    m_isExitButtonSelected = true;
                }
                break;
            case sf::Keyboard::Return:
                m_isHostButtonPressed = m_isHostButtonSelected;
                m_isClientButtonPressed = m_isClientButtonSelected;
                m_isExitButtonPressed = m_isExitButtonSelected;
                break;
            default:
                break;
            }
        }
    }
}


void MultiplayerMenuState::Update(const sf::Time& deltaTime) {
    if (m_isHostButtonSelected) {
        m_hostButton.setFillColor(sf::Color::Yellow);
        m_clientButton.setFillColor(sf::Color::White);
        m_exitButton.setFillColor(sf::Color::White);
    }
    else if (m_isClientButtonSelected) {
        m_hostButton.setFillColor(sf::Color::White);
        m_clientButton.setFillColor(sf::Color::Yellow);
        m_exitButton.setFillColor(sf::Color::White);
    }
    else
    {
        m_hostButton.setFillColor(sf::Color::White);
        m_clientButton.setFillColor(sf::Color::White);
        m_exitButton.setFillColor(sf::Color::Yellow);
    }

    // Handle button press actions
    if (m_isHostButtonPressed) {
        // Example: Transition to a "host game" state
        m_context->m_states->Add(std::make_unique<ServerGameState>(m_context, m_networkManager), true);
        m_isHostButtonPressed = false; // Reset the button pressed state
    }
    else if (m_isClientButtonPressed) {
        // Example: Transition to a "join game" state
        m_context->m_states->Add(std::make_unique<ClientGameState>(m_context, m_networkManager), true);
        m_isClientButtonPressed = false; // Reset the button pressed state
    }
    else if (m_isExitButtonPressed) {
        m_isExitButtonPressed = false;
    }
}

