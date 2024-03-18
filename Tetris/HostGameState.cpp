#include "HostGameState.h"
#include <enet/enet.h>
#include <iostream>

void HostGameState::Init() {
    if (m_networkManager && m_networkManager->IsConnected()) {
        // Faz algo se j� estiver conectado
        std::cout << "Already connected!" << std::endl;
    }
    else {
        // Tenta iniciar o host se ainda n�o estiver conectado
        m_networkManager->StartHost(1234);
    }
}

void HostGameState::ProcessInput() {
    //sf::Event event;
    //while (m_context->m_window->pollEvent(event)) {
    //    if (event.type == sf::Event::Closed) {
    //        m_context->m_window->close();
    //    }
    //    // Aqui voc� pode adicionar mais l�gica para outros tipos de entrada
    //}
}

void HostGameState::Update(const sf::Time& deltaTime) {
    // Atualiza��es espec�ficas deste estado v�o aqui
    // Por exemplo, voc� pode querer processar eventos de rede
    std::cout << "getclients" << m_networkManager->getClientIDs().size() << std::endl;
    std::cout << "ingameplaystate" << currentyInGameplayState << std::endl;
    std::cout << "isconnected" << m_networkManager->IsConnected() << std::endl;
    if (m_networkManager->IsConnected() && !currentyInGameplayState && m_networkManager->getClientIDs().size() != NULL) {
        std::cout << "N�O ESTOU EM GAMEPLAY E UM CLIENTE CONECTOU" << std::endl;
        std::map<ENetPeer*, int> clientIDs = m_networkManager->getClientIDs();
        MenuIdle();
    }
    m_networkManager->ProcessNetworkEvents();
    // Aqui pode ser um bom lugar para verificar conex�es de clientes e talvez mudar o estado do jogo
}

void HostGameState::Draw() {
    // Desenha a interface do usu�rio espec�fica deste estado aqui
    m_context->m_window->clear(sf::Color::Blue); // Exemplo: Limpa a tela com preto

    // Aqui voc� pode desenhar elementos espec�ficos do host, como informa��es de conex�o
    if (m_networkManager->IsConnected() && !currentyInGameplayState && m_networkManager->getClientIDs().size() != NULL) {
        m_context->m_window->draw(m_gameTitle);
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
}
