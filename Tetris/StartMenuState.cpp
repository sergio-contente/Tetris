#include "StartMenuState.h"
#include "GamePlayState.h"

StartMenuState::StartMenuState(std::shared_ptr <Context>& context, std::shared_ptr<NetworkManager>& networkManager) :
	m_context(context),
	m_isPlayButtonSelected(true),
	m_isPlayButtonPressed(false),
	m_isExitButtonPressed(false),
	m_isExitButtonSelected(false),
	m_isMultiplayerButtonSelected(false),
	m_isMultiplayerButtonPressed(false),
	m_networkManager(networkManager)
{}

StartMenuState::~StartMenuState()
{}

void StartMenuState::Init() {
	m_context->m_assets->LoadFont("Start_Menu_Font", "assets/Computerfont.ttf");

	//Title
	m_gameTitle.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
	m_gameTitle.setString("Tetris");
	m_gameTitle.setOrigin(m_gameTitle.getLocalBounds().width / 2, m_gameTitle.getLocalBounds().height / 2);
	m_gameTitle.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 - 150.f);

	//Play Button
	m_playButton.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
	m_playButton.setString("Play");
	m_playButton.setOrigin(m_playButton.getLocalBounds().width / 2, m_playButton.getLocalBounds().height / 2);
	m_playButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 - 50.f);
	m_playButton.setCharacterSize(20.f);

	// Multiplayer Button
	m_multiplayerButton.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
	m_multiplayerButton.setString("Multiplayer");
	m_multiplayerButton.setOrigin(m_multiplayerButton.getLocalBounds().width / 2, m_multiplayerButton.getLocalBounds().height / 2);
	m_multiplayerButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2);
	m_multiplayerButton.setCharacterSize(20.f);

	//Exit Button
	m_exitButton.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
	m_exitButton.setString("Exit");
	m_exitButton.setOrigin(m_exitButton.getLocalBounds().width / 2, m_exitButton.getLocalBounds().height / 2);
	m_exitButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 + 50.f);
	m_exitButton.setCharacterSize(20.f);
}


void StartMenuState::Draw() {
	m_context->m_window->clear(sf::Color::Blue);
	m_context->m_window->draw(m_gameTitle);
	m_context->m_window->draw(m_playButton);
	m_context->m_window->draw(m_exitButton);
	m_context->m_window->draw(m_multiplayerButton);
	m_context->m_window->display();
}

void StartMenuState::ProcessInput() {
	sf::Event event;
	while (m_context->m_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			m_context->m_window->close();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Up:
				if (m_isExitButtonSelected)
				{
					m_isExitButtonSelected = false;
					m_isMultiplayerButtonSelected = true;
				}
				else if (m_isMultiplayerButtonSelected)
				{
					m_isMultiplayerButtonSelected = false;
					m_isPlayButtonSelected = true;
				}
				break;
			case sf::Keyboard::Down:
				if (m_isPlayButtonSelected)
				{
					m_isPlayButtonSelected = false;
					m_isMultiplayerButtonSelected = true;
				}
				else if (m_isMultiplayerButtonSelected)
				{
					m_isMultiplayerButtonSelected = false;
					m_isExitButtonSelected = true;
				}
				break;
			case sf::Keyboard::Return:
				m_isPlayButtonPressed = m_isPlayButtonSelected;
				m_isExitButtonPressed = m_isExitButtonSelected;
				m_isMultiplayerButtonPressed = m_isMultiplayerButtonSelected;
				break;
			default:
				break;
			}
		}
	}
}

void StartMenuState::Update(const sf::Time& deltaTime) {
	if (m_isPlayButtonSelected)
	{
		m_playButton.setFillColor(sf::Color::Yellow);
		m_multiplayerButton.setFillColor(sf::Color::White);
		m_exitButton.setFillColor(sf::Color::White);
	}
	else if (m_isMultiplayerButtonSelected)
	{
		m_playButton.setFillColor(sf::Color::White);
		m_multiplayerButton.setFillColor(sf::Color::Yellow);
		m_exitButton.setFillColor(sf::Color::White);
	}
	else if (m_isExitButtonSelected)
	{
		m_playButton.setFillColor(sf::Color::White);
		m_multiplayerButton.setFillColor(sf::Color::White);
		m_exitButton.setFillColor(sf::Color::Yellow);
	}

	// Handle button press actions
	if (m_isPlayButtonPressed)
	{
		m_context->m_states->Add(std::make_unique<GamePlayState>(m_context, m_networkManager), false);
		m_isPlayButtonPressed = false; // Reset the button pressed state
	}
	else if (m_isMultiplayerButtonPressed)
	{
		// Here, you would transition to your multiplayer state or setup screen
		m_context->m_states->Add(std::make_unique<MultiplayerMenuState>(m_context, m_networkManager), true);
		m_isMultiplayerButtonPressed = false; // Reset the button pressed state
	}
	else if (m_isExitButtonPressed)
	{
		m_context->m_states->PopAll();
		m_isExitButtonPressed = false; // Reset the button pressed state
	}
}