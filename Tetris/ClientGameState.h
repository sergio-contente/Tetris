#pragma once

#include "State.h"
#include "Game.h"
#include "NetworkManager.h"
#include <memory>

class ClientGameState : public State {
private:
    std::shared_ptr<Context> m_context;
    std::shared_ptr<NetworkManager> m_networkManager;

public:
    ClientGameState(std::shared_ptr<Context>& context, std::shared_ptr<NetworkManager>& networkManager);
    virtual ~ClientGameState();

    void Init() override;
    void ProcessInput() override;
    void Update(const sf::Time& deltaTime) override;
    void Draw() override;
};
