#ifndef __STATE_H__
#define __STATE_H__

#include "SFML/System/Time.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class StateManager;

class State
{
	public:
		State(const StateManager& stateManager,  sf::RenderWindow& window) : m_stateManager(stateManager), m_window(window) {}
		virtual ~State() {};
		virtual void Init() = 0;
		virtual void Cleanup() = 0;

		virtual void Pause() = 0;
		virtual void Resume() = 0;

		virtual void HandleEvents() = 0;
		virtual void Update(sf::Time deltaTime) = 0;
		virtual void Draw() = 0;
	protected:
		const StateManager& m_stateManager;
		const sf::RenderWindow m_window;
};

#endif // !__STATE_H__



