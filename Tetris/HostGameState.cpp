#include "HostGameState.h"
#include <iostream>

void HostGameState::Init() {
    if (m_networkManager && m_networkManager->IsConnected()) {
        // Faz algo se j� estiver conectado
    }
    else {
        // Tenta iniciar o host se ainda n�o estiver conectado
        m_networkManager->StartHost(1234);
    }
}

void HostGameState::ProcessInput() {
    sf::Event event;
    while (m_context->m_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_context->m_window->close();
        }
        // Aqui voc� pode adicionar mais l�gica para outros tipos de entrada
    }
}

void HostGameState::Update(const sf::Time& deltaTime) {
    // Atualiza��es espec�ficas deste estado v�o aqui
    // Por exemplo, voc� pode querer processar eventos de rede
    m_networkManager->ProcessNetworkEvents();

    // Aqui pode ser um bom lugar para verificar conex�es de clientes e talvez mudar o estado do jogo
}

void HostGameState::Draw() {
    // Desenha a interface do usu�rio espec�fica deste estado aqui
    m_context->m_window->clear(sf::Color::Black); // Exemplo: Limpa a tela com preto

    // Aqui voc� pode desenhar elementos espec�ficos do host, como informa��es de conex�o

    m_context->m_window->display(); // Mostra o que foi desenhado
}
