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

	// Load the logo image
	m_context->m_assets->LoadTexture("Logo", "assets/Logo.png");
	// Load the background Texture
	m_context->m_assets->LoadTexture("Background", "assets/Background.png");

	// Get the texture and set it to be repeated
	sf::Texture& backgroundTexture = m_context->m_assets->GetTexture("Background");
	backgroundTexture.setRepeated(true);

	// Create the background sprite
	m_backgroundSprite.setTexture(backgroundTexture);
	// Set the texture rect to cover the whole screen
	m_backgroundSprite.setTextureRect(sf::IntRect(0, 0, m_context->m_window->getSize().x, m_context->m_window->getSize().y));



	// Create the logo sprite
	m_logoSprite.setTexture(m_context->m_assets->GetTexture("Logo"));
	// Set the origin of the logo sprite to its center for proper positioning
	m_logoSprite.setOrigin(m_logoSprite.getLocalBounds().width / 2, m_logoSprite.getLocalBounds().height / 2);
	// Position the logo sprite at the center-top of the screen
	m_logoSprite.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 4);

	// Scale the logo sprite based on screen resolution (example scaling factor)
	//float scaleX = m_context->m_window->getSize().x / 1920.f; // Assuming 1920 is the reference width
	//float scaleY = m_context->m_window->getSize().y / 1080.f; // Assuming 1080 is the reference height
	float scaleX = 1.f; // Assuming 1920 is the reference width
	float scaleY = 1.f; // Assuming 1080 is the reference height
	m_logoSprite.setScale(scaleX, scaleY);


	//Title
	//m_gameTitle.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
	//m_gameTitle.setString("Tetris");
	//m_gameTitle.setOrigin(m_gameTitle.getLocalBounds().width / 2, m_gameTitle.getLocalBounds().height / 2);
	//m_gameTitle.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 - 150.f);

	// To resize the buttons according to the screen size
	float buttonScale = std::min(scaleX, scaleY);

	//Play Button
	m_playButton.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
	m_playButton.setString("Play");
	m_playButton.setCharacterSize(static_cast<unsigned int>(60.f * buttonScale));
	m_playButton.setOrigin(m_playButton.getLocalBounds().width / 2, m_playButton.getLocalBounds().height / 2);
	m_playButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 * 0.95);

	// Multiplayer Button
	m_multiplayerButton.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
	m_multiplayerButton.setString("Multiplayer");
	m_multiplayerButton.setCharacterSize(static_cast<unsigned int>(60.f * buttonScale));
	m_multiplayerButton.setOrigin(m_multiplayerButton.getLocalBounds().width / 2, m_multiplayerButton.getLocalBounds().height / 2);
	m_multiplayerButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 * 1.15);

	//Exit Button
	m_exitButton.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
	m_exitButton.setString("Exit");
	m_exitButton.setCharacterSize(static_cast<unsigned int>(60.f * buttonScale));
	m_exitButton.setOrigin(m_exitButton.getLocalBounds().width / 2, m_exitButton.getLocalBounds().height / 2);
	m_exitButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 * 1.35);


}


void StartMenuState::Draw() {
	m_context->m_window->clear(sf::Color::Blue);
	// Draw the background sprite first
	m_context->m_window->draw(m_backgroundSprite);

	// Draw the logo sprite before the buttons
	m_context->m_window->draw(m_logoSprite);

	// Draw buttons as before
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
		m_context->m_states->Add(std::make_unique<MultiplayerMenuState>(m_context, m_networkManager), false);
		m_isMultiplayerButtonPressed = false; // Reset the button pressed state
	}
	else if (m_isExitButtonPressed)
	{
		m_context->m_states->PopAll();
		m_isExitButtonPressed = false; // Reset the button pressed state
	}

	// Add code to move the background
	static float backgroundX = 0.f; // These static variables will keep their values between updates
	static float backgroundY = 0.f;
	const float backgroundSpeedX = 100.f; // Speed of the background movement along X-axis
	const float backgroundSpeedY = 50.f;  // Speed of the background movement along Y-axis

	// Calculate new positions based on deltaTime (might get an overflow if the game runs for a long time)
	backgroundX += backgroundSpeedX * deltaTime.asSeconds();
	backgroundY += backgroundSpeedY * deltaTime.asSeconds();

	m_backgroundSprite.setTextureRect(sf::IntRect(static_cast<int>(backgroundX), static_cast<int>(backgroundY), m_context->m_window->getSize().x, m_context->m_window->getSize().y));

}