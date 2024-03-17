#include "ClientGameState.h"
#include <iostream>

ClientGameState::ClientGameState(std::shared_ptr<Context>& context, std::shared_ptr<NetworkManager>& networkManager)
    : m_context(context), m_networkManager(networkManager) {
}

ClientGameState::~ClientGameState() {
    // Adicione aqui qualquer l�gica de limpeza necess�ria
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
    sf::Event event;
    while (m_context->m_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_context->m_window->close();
        }
        // Adicione mais l�gica de entrada conforme necess�rio
    }
}

void ClientGameState::Update(const sf::Time& deltaTime) {
    // Processa eventos de rede e atualiza o estado do jogo
    m_networkManager->ProcessNetworkEvents();
    // Adicione aqui qualquer l�gica de atualiza��o espec�fica do cliente
}

void ClientGameState::Draw() {
    m_context->m_window->clear(sf::Color::Blue); // Exemplo: Limpa a tela com azul

    // Adicione aqui o desenho da interface do usu�rio espec�fica do cliente

    m_context->m_window->display(); // Mostra o que foi desenhado
}
