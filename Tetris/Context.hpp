#ifndef CONTEXT_HPP
#define CONTEXT_HPP

//#define NOMINMAX
//#include <windows.h>

#include <memory>
#include "SFML/Graphics.hpp"
#include "StateManager.h"
#include "AssetManager.h"

struct Context
{
    std::shared_ptr<float> m_backgroundX;
    std::shared_ptr<float> m_backgroundY;
    std::shared_ptr<sf::RenderWindow> m_window;
    std::shared_ptr<StateManager> m_states;
    std::shared_ptr<AssetManager> m_assets;

    Context()
    {
        m_backgroundX = std::make_shared<float>();
        m_backgroundY = std::make_shared<float>();
        m_window = std::make_shared<sf::RenderWindow>();
        m_states = std::make_shared<StateManager>();
        m_assets = std::make_shared<AssetManager>();
    }
};

#endif // CONTEXT_HPP
