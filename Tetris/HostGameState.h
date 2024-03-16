#ifndef HOSTGAMESTATE_H
#define HOSTGAMESTATE_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "State.h"
#include "Game.h"

class HostGameState : public State {
private:

    std::shared_ptr<Context> m_context;
    std::unique_ptr<NetworkManager> m_networkManager;

public:
    HostGameState(std::shared_ptr<Context>& context)
        : m_context(context) {
        m_networkManager = std::make_unique<NetworkManager>();
    }

    ~HostGameState(){}

    void Init() override {
        m_networkManager->StartHost();
    }

    void ProcessInput() override {
        // Processa entradas espec�ficas deste estado
    }

    void Update(const sf::Time& deltaTime) override {
        // Atualiza��es espec�ficas deste estado
        m_networkManager->ProcessNetworkEvents();
    }

    void Draw() override {
        // Desenhar a interface do usu�rio espec�fica deste estado, se houver
    }
};

#endif // HOSTGAMESTATE_H