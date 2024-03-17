#include "HostGameState.h"
#include <iostream>

void HostGameState::Init() {
    if (m_networkManager && m_networkManager->IsConnected()) {
        // Faz algo se já estiver conectado
    }
    else {
        // Tenta iniciar o host se ainda não estiver conectado
        m_networkManager->StartHost(1234);
    }
}

void HostGameState::ProcessInput() {
    sf::Event event;
    while (m_context->m_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_context->m_window->close();
        }
        // Aqui você pode adicionar mais lógica para outros tipos de entrada
    }
}

void HostGameState::Update(const sf::Time& deltaTime) {
    // Atualizações específicas deste estado vão aqui
    // Por exemplo, você pode querer processar eventos de rede
    m_networkManager->ProcessNetworkEvents();

    // Aqui pode ser um bom lugar para verificar conexões de clientes e talvez mudar o estado do jogo
}

void HostGameState::Draw() {
    // Desenha a interface do usuário específica deste estado aqui
    m_context->m_window->clear(sf::Color::Black); // Exemplo: Limpa a tela com preto

    // Aqui você pode desenhar elementos específicos do host, como informações de conexão

    m_context->m_window->display(); // Mostra o que foi desenhado
}
