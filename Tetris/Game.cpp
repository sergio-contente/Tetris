#include "Game.h"
#include "StartMenuState.h"


Game::Game() : m_context(std::make_shared<Context>()) {
    m_context->m_window->create(sf::VideoMode(640, 352), "Tetris", sf::Style::Close);
    m_context->m_states->Add(std::make_unique<StartMenuState>(m_context, m_networkManager.get()), false);
}

Game::~Game()
{}

void Game::Run()
{
    sf::Clock clock;
    sf::Time timeSinceLastFrame = sf::Time::Zero;

    while (m_context->m_window->isOpen())
    {
        timeSinceLastFrame += clock.restart();

        while (timeSinceLastFrame > TIME_PER_FRAME)
        {
            timeSinceLastFrame -= TIME_PER_FRAME;

            m_context->m_states->ProcessStateChange();

            if (!m_context->m_states->IsEmpty())
            {
                m_context->m_states->GetCurrent()->ProcessInput();
                m_context->m_states->GetCurrent()->Update(TIME_PER_FRAME);
                m_context->m_states->GetCurrent()->Draw();

                if (m_networkManager->IsConnected()) {
                    m_networkManager->ProcessNetworkEvents();
                }
            }
            else
            {
                m_context->m_window->close();
            }
        }
    }
}