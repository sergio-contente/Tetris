#pragma once

#include <SFML/Graphics.hpp>

#include "State.h"
#include "Game.h"

class GamePlayState : public State
{
private:
	std::shared_ptr <Context>m_context;
public:
	GamePlayState();
	~GamePlayState();
};

