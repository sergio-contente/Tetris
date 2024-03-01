#include "Game.h"
#include <SFML/Window/Event.hpp>

Game::Game() : m_context(std::make_shared<Context>())
{
    m_context->m_window->create(sf::VideoMode(640, 352), "Tetris", sf::Style::Close);
    //Add first state to m_states here
}

Game::~Game()
{
    // Limpeza, se necessário...
}

void Game::Run()
{
    sf::Clock clock;

    while (m_context->m_window->isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        m_context->m_states->HandleEvents();

        if (!m_context->m_states->IsEmpty())
        {
            m_context->m_states->HandleEvents();
            m_context->m_states->Update(elapsedTime);
            m_context->m_window->clear();
            m_context->m_states->Draw();
            m_context->m_window->display();
        }
        else
        {
            m_context->m_window->close();
        }
    }
}
