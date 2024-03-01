#pragma once

#include<memory>
#include<SFML/Graphics/Sprite.hpp>

#include "State.h"
#include "Game.h"

class GamePlay : public State
{
private:
	std::shared_ptr<Context> m_context;
public:
	GamePlay(std::shared_ptr<Context>& context);
	~GamePlay();

	void Init() override;
	void Cleanup() override;

	void Pause() override;
	void Resume() override;

	void HandleEvents() override;
	void Update(sf::Time deltaTime) override;
	void Draw() override;
};

