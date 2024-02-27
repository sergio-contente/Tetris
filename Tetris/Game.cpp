#include "Game.h"
#include "MainMenu.hpp"
#include <SFML/Graphics/CircleShape.hpp>

Game::Game() : m_context(std::make_shared<Context>())
{
    m_context->m_window->create(sf::VideoMode(640, 352), "Snake Game", sf::Style::Close);
    m_context->m_states->PushState(std::make_unique<MainMenu>(m_context));
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
