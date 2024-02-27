#ifndef __STATE_H__
#define __STATE_H__

#include "SFML/System/Time.hpp"

class StateManager;

class State
{
	public:
		State() {};
		virtual ~State() {};
		virtual void Init() = 0;
		virtual void Cleanup() = 0;

		virtual void Pause() = 0;
		virtual void Resume() = 0;

		virtual void HandleEvents() = 0;
		virtual void Update(sf::Time deltaTime) = 0;
		virtual void Draw() = 0;
	
};

#endif // !__STATE_H__



