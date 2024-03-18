#include "ClientGameState.h"
#include <enet/enet.h>
#include <iostream>

ClientGameState::ClientGameState(std::shared_ptr<Context>& context, std::shared_ptr<NetworkManager>& networkManager)
    : m_context(context), m_networkManager(networkManager) {
}

ClientGameState::~ClientGameState() {
    // Adicione aqui qualquer lógica de limpeza necessária
}

void ClientGameState::Init() {
    // Tenta conectar ao servidor; substitua "localhost" e "1234" conforme apropriado
    if (!m_networkManager->StartClient("localhost", 1234)) {
        std::cerr << "Failed to connect to the host." << std::endl;
    }
    else {
        std::cout << "Successfully connected to the host." << std::endl;
    }
}

void ClientGameState::ProcessInput() {
    //sf::Event event;
    //while (m_context->m_window->pollEvent(event)) {
    //    if (event.type == sf::Event::Closed) {
    //        m_context->m_window->close();
    //    }
    //    // Adicione mais lógica de entrada conforme necessário
    //}
}

void ClientGameState::Update(const sf::Time& deltaTime) {
    // Processa eventos de rede e atualiza o estado do jogo
    // Adicione aqui qualquer lógica de atualização específica do cliente
    std::cout << "peer_client" << m_networkManager->peer << std::endl;
    std::cout << "client_client" << m_networkManager->client << std::endl;
    currentyInGameplayState = m_networkManager->isReadyToStartGame();
    if (m_networkManager->IsConnected() && currentyInGameplayState) {
        m_context->m_states->Add(std::make_unique<GamePlayState>(m_context, m_networkManager), false);
    }
     m_networkManager->ProcessNetworkEvents();
}

void ClientGameState::Draw() {
    m_context->m_window->clear(sf::Color::Blue); // Exemplo: Limpa a tela com azul

    // Adicione aqui o desenho da interface do usuário específica do cliente

    m_context->m_window->display(); // Mostra o que foi desenhado
}
