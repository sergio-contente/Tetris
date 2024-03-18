#include <SFML/Graphics.hpp>
#include "Game.h"
#include "StartMenuState.h"

Game::Game() : m_context(std::make_shared<Context>())
{
    // Get the desktop resolution
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // Create the window in full screen mode with the desktop resolution
    m_context->m_window->create(desktop, "Tetris", sf::Style::Fullscreen);

    m_context->m_states->Add(std::make_unique<StartMenuState>(m_context));
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

                /*if (m_networkManager->IsConnected()) {
                    m_networkManager->ProcessNetworkEvents();
                }*/
            }
            else
            {
                m_context->m_window->close();
            }
        }
    }
}
