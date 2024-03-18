#ifndef GAME_HPP
#define GAME_HPP

//#define NOMINMAX
//#include <windows.h>

#include <memory>
#include "Context.hpp"
#include "NetworkManager.h"

class Game
{
public:
    Game();
    ~Game();

    void Run();

private:
    std::shared_ptr<Context> m_context;
    std::shared_ptr<NetworkManager> m_networkManager;
    const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f);
};

#endif // GAME_HPP
