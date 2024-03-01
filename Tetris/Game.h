#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include "Context.hpp"
// Outras inclusões necessárias...

class Game
{
public:
    Game();
    ~Game();

    void Run();

private:
    std::shared_ptr<Context> m_context;
    const sf::Time TIME_PER_SECOND = sf::seconds(1.f / 60.f);
};

#endif // GAME_HPP
