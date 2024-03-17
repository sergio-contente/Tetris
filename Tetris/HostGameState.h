#ifndef HOSTGAMESTATE_H
#define HOSTGAMESTATE_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "State.h"
#include "Game.h"

class HostGameState : public State {
private:

    std::shared_ptr<Context> m_context;
    NetworkManager* m_networkManager;

public:
    HostGameState(std::shared_ptr<Context>& context, NetworkManager* m_networkManager)
        : m_context(context),
        m_networkManager(m_networkManager)
    {}

    ~HostGameState(){}

    void Init() override;

    void ProcessInput() override;

    void Update(const sf::Time& deltaTime) override;

    void Draw() override;
};

#endif // HOSTGAMESTATE_H