#ifndef STARTMENUSTATE_H
#define STARTMENUSTATE_H

//#define NOMINMAX
//#include <windows.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "State.h"
#include "Game.h"
#include "MultiplayerMenuState.h"

class StartMenuState : public State
{
private:
	std::shared_ptr <Context>m_context;
	sf::Text m_gameTitle;
	sf::Text m_playButton;
	sf::Text m_exitButton;
	sf::Texture m_backgroundTexture;
	sf::Sprite m_backgroundSprite;
	sf::Sprite m_logoSprite;

	sf::Text m_multiplayerButton;

	bool m_isPlayButtonSelected;
	bool m_isPlayButtonPressed;

	bool m_isExitButtonSelected;
	bool m_isExitButtonPressed;

	bool m_isMultiplayerButtonSelected;
	bool m_isMultiplayerButtonPressed;

	std::shared_ptr<NetworkManager> m_networkManager;


public:
	StartMenuState(std::shared_ptr <Context>& context, std::shared_ptr<NetworkManager>& networkManager);
	~StartMenuState();

	void Init() override;
	void ProcessInput() override;
	void Update(const sf::Time& deltaTime) override;
	void Draw() override;
};
#endif


