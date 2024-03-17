#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "State.h"
#include "Game.h"
#include "GamePlayState.h"
#include "StartMenuState.h"
#include "HostGameState.h"
#include "ClientGameState.h"

class MultiplayerMenuState : public State
{
private:
	std::shared_ptr <Context>m_context;
	sf::Text m_gameTitle;
	sf::Text m_hostButton;
	sf::Text m_clientButton;
	sf::Text m_exitButton;

	bool m_isHostButtonSelected;
	bool m_isHostButtonPressed;

	bool m_isClientButtonSelected;
	bool m_isClientButtonPressed;

	bool m_isExitButtonSelected;
	bool m_isExitButtonPressed;

	NetworkManager* m_networkManager;

public:
	MultiplayerMenuState(std::shared_ptr <Context>& m_context, NetworkManager* m_networkManager);
	~MultiplayerMenuState();

	void Init() override;
	void ProcessInput() override;
	void Update(const sf::Time& deltaTime) override;
	void Draw() override;
};

