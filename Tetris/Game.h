#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include "Context.hpp"
// Outras inclus�es necess�rias...

class Game
{
public:
    Game();
    ~Game();

    void Run();

private:
    std::shared_ptr<Context> m_context;
    // Outros membros necess�rios...
};

#endif // GAME_HPP
