#pragma once

#include <iostream>
#include <vector>
#include "GameState.h"

class GameEngine
{
public:
	void Init();
	void Cleanup();

	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return m_running; }
	void Quit() { m_running = false; }

private:
	// the stack of states
	std::vector<GameState*> states;

	bool m_running;
};

