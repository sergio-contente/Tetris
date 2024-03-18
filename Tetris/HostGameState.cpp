#include "HostGameState.h"
#include <enet/enet.h>
#include <iostream>

void HostGameState::Init() {
    if (m_networkManager && m_networkManager->IsConnected()) {
        // Faz algo se já estiver conectado
        std::cout << "Already connected!" << std::endl;
    }
    else {
        // Tenta iniciar o host se ainda não estiver conectado
        m_networkManager->StartHost(1234);
    }
}

void HostGameState::ProcessInput() {
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

void HostGameState::Update(const sf::Time& deltaTime) {

    // Atualizações específicas deste estado vão aqui
    // Por exemplo, você pode querer processar eventos de rede
    /*std::cout << "getclients" << m_networkManager->getClientIDs().size() << std::endl;
    std::cout << "ingameplaystate" << currentyInGameplayState << std::endl;
    std::cout << "isconnected" << m_networkManager->IsConnected() << std::endl;*/
    std::cout <<"peer_host" << m_networkManager->peer << std::endl;
    std::cout <<"client_host" <<  m_networkManager->client << std::endl;
    currentyInGameplayState = m_networkManager->isReadyToStartGame();
    if (m_networkManager->IsConnected()  && m_networkManager->getClientIDs().size() != NULL) {
        if (!currentyInGameplayState)
        {
            std::map<ENetPeer*, int> clientIDs = m_networkManager->getClientIDs();
            MenuIdle();
        }
        else {
            m_context->m_states->Add(std::make_unique<GamePlayState>(m_context, m_networkManager), false);
        }

    }
    m_networkManager->ProcessNetworkEvents();
    // Aqui pode ser um bom lugar para verificar conexões de clientes e talvez mudar o estado do jogo
}

void HostGameState::Draw() {
    // Desenha a interface do usuário específica deste estado aqui
    m_context->m_window->clear(sf::Color::Blue); // Exemplo: Limpa a tela com preto

    // Aqui você pode desenhar elementos específicos do host, como informações de conexão
    if (m_networkManager->IsConnected()  && m_networkManager->getClientIDs().size() != NULL) {
        if (currentyInGameplayState) {
            m_playButton.setString("Waiting for other player...");
        }
            m_context->m_window->draw(m_gameTitle);
            m_context->m_window->draw(m_playButton);
    }
    m_context->m_window->display(); // Mostra o que foi desenhado
}

void HostGameState::MenuIdle() {
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
