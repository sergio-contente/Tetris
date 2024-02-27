#ifndef __STATEMANAGER_H__
#define __STATEMANAGER_H__

#include <iostream>
#include <vector>

#include "State.h"

class StateManager
{
public:
	StateManager();
	~StateManager();


	void ChangeState(State* state);
	void PushState(State* state);
	void PopState();

	void HandleEvents();
	void Update(sf::Time deltaTime);
	void Draw();

	bool Running() { return m_running; }
	void Quit() { m_running = false; }

	bool IsEmpty() const;

private:
	// the stack of states
	std::vector<State*> states;

	bool m_running;
};

#endif // !__STATEMANAGER_H__

