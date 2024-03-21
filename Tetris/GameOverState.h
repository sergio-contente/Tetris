#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

//#include <windows.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "State.h"
#include "Game.h"
#include "GamePlayState.h"
#include "StartMenuState.h"

class GameOverState : public State
{
	private:
		std::shared_ptr <Context>m_context;
		std::shared_ptr <NetworkManager> m_networkManager;

		sf::Text m_gameTitle;
		sf::Text m_gameOver;
		sf::Text m_HighScoreText;
		sf::Text m_retryButton;
		sf::Text m_mainMenuButton;

		bool m_isRetryButtonSelected;
		bool m_isRetryButtonPressed;

		bool m_isMainMenuButtonSelected;
		bool m_isMainMenuButtonPressed;

		int m_HighScore;


	public:
		GameOverState(std::shared_ptr <Context>& m_context, std::shared_ptr <NetworkManager>& m_networkManager, int lastHighScore);
		~GameOverState();

		void Init() override;
		void ProcessInput() override;
		void Update(const sf::Time& deltaTime) override;
		void Draw() override;
};

#endif
