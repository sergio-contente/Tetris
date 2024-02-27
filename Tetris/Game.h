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
    // Outros membros necessários...
};

#endif // GAME_HPP
