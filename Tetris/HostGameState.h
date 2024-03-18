#ifndef HOSTGAMESTATE_H
#define HOSTGAMESTATE_H

//#define NOMINMAX
//#include <windows.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "State.h"
#include "Game.h"
#include "GamePlayState.h"
#include "AssetManager.h"


class HostGameState : public State {
private:
    sf::Text m_gameTitle;
    sf::Text m_playButton;
    sf::Text m_client;

    std::shared_ptr<Context> m_context;
    std::shared_ptr<NetworkManager> m_networkManager;

    bool currentyInGameplayState = false;
    std::map<ENetPeer*, int> clientIDs;

public:
    HostGameState(std::shared_ptr<Context>& context, std::shared_ptr<NetworkManager>& m_networkManager)
        : m_context(context),
        m_networkManager(m_networkManager)
    {}

    ~HostGameState(){}

    void Init() override;

    void ProcessInput() override;

    void Update(const sf::Time& deltaTime) override;

    void Draw() override;

    void MenuIdle();
};

#endif // HOSTGAMESTATE_H