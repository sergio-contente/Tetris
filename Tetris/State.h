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
        virtual void ProcessInput() = 0;
        virtual void Update(const sf::Time& deltaTime) = 0;
        virtual void Draw() = 0;

        virtual void Pause() {};
        virtual void Start() {};
	
};

#endif // !__STATE_H__



