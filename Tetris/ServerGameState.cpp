#include "ServerGameState.h"
#include <enet/enet.h>
#include <iostream>

void ServerGameState::Init() {
    if (m_networkManager && m_networkManager->IsConnected()) {
        // Faz algo se já estiver conectado
        std::cout << "Already connected!" << std::endl;
    }
    else {
        // Tenta iniciar o Server se ainda não estiver conectado
        m_networkManager->StartHost(1234);
    }
}

void ServerGameState::ProcessInput() {
	sf::Event event;
	while (m_context->m_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			m_context->m_window->close();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Return:
				m_isPlayButtonPressed = m_isPlayButtonSelected;
				break;
			default:
				break;
			}
		}
	}
}

void ServerGameState::Update(const sf::Time& deltaTime) {
    currentyInGameplayState = m_networkManager->isReadyToStartGame();
    if (m_networkManager->IsConnected()  && m_networkManager->getClients() != NULL) {
        if (!currentyInGameplayState)
        {
            MenuIdle();
        }
        else {
            m_context->m_states->Add(std::make_unique<GamePlayState>(m_context, m_networkManager), false);
        }

    }
    m_networkManager->ProcessNetworkEvents();
}

void ServerGameState::Draw() {
    // Desenha a interface do usuário específica deste estado aqui
    m_context->m_window->clear(sf::Color::Blue); // Exemplo: Limpa a tela com preto

    // Aqui você pode desenhar elementos específicos do Server, como informações de conexão
    if (m_networkManager->IsConnected()  && m_networkManager->getClients() != NULL) {
        if (currentyInGameplayState) {
            m_playButton.setString("Waiting for other player...");
        }
            m_context->m_window->draw(m_gameTitle);
            m_context->m_window->draw(m_playButton);
    }
    m_context->m_window->display(); // Mostra o que foi desenhado
}

void ServerGameState::MenuIdle() {
    m_context->m_assets->LoadFont("Start_Menu_Font", "assets/Computerfont.ttf");

    //Title
    m_gameTitle.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
    m_gameTitle.setString("Tetris");
    m_gameTitle.setOrigin(m_gameTitle.getLocalBounds().width / 2, m_gameTitle.getLocalBounds().height / 2);
    m_gameTitle.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 - 150.f);

    //Play Button
    m_playButton.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
    m_playButton.setString("Play");
    m_playButton.setOrigin(m_playButton.getLocalBounds().width / 2, m_playButton.getLocalBounds().height / 2);
    m_playButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 - 50.f);
    m_playButton.setCharacterSize(20.f);

    // Handle button press actions
    if (m_isPlayButtonPressed)
    {
        m_isPlayButtonPressed = false; // Reset the button pressed state
        m_networkManager->notifyGameReady();
    }
}
