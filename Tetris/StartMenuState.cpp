#include "StartMenuState.h"
#include "GamePlayState.h"

StartMenuState::StartMenuState(std::shared_ptr <Context> &context) : m_context(context), m_isPlayButtonSelected(true),
m_isPlayButtonPressed(false), m_isExitButtonPressed(false), m_isExitButtonSelected(false)
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
	float scaleX = m_context->m_window->getSize().x / 1920.f; // Assuming 1920 is the reference width
	float scaleY = m_context->m_window->getSize().y / 1080.f; // Assuming 1080 is the reference height
	m_logoSprite.setScale(scaleX, scaleY);

	//Title
	m_gameTitle.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
	m_gameTitle.setString("Tetris");
	m_gameTitle.setOrigin(m_gameTitle.getLocalBounds().width / 2, m_gameTitle.getLocalBounds().height / 2);
	m_gameTitle.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 - 150.f);
	//Play Button
	m_playButton.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
	m_playButton.setString("Play");
	m_playButton.setOrigin(m_playButton.getLocalBounds().width / 2, m_playButton.getLocalBounds().height / 2);
	m_playButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 - 25.f);
	m_playButton.setCharacterSize(20.f);
	//Exit Button
	m_exitButton.setFont(m_context->m_assets->GetFont("Start_Menu_Font"));
	m_exitButton.setString("Exit");
	m_exitButton.setOrigin(m_exitButton.getLocalBounds().width / 2, m_exitButton.getLocalBounds().height / 2);
	m_exitButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 + 25.f);
	m_exitButton.setCharacterSize(20.f);

	// For Play and Exit buttons, you can adjust character size or scale them similarly to the logo sprite
	float buttonScale = std::min(scaleX, scaleY); // You might want to scale buttons less aggressively
	m_playButton.setCharacterSize(static_cast<unsigned int>(20.f * buttonScale));
	m_exitButton.setCharacterSize(static_cast<unsigned int>(20.f * buttonScale));
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
			{
				if (!m_isPlayButtonSelected)
				{
					m_isPlayButtonSelected = true;
					m_isExitButtonSelected = false;
				}
				break;
			}
			case sf::Keyboard::Down:
			{
				if (!m_isExitButtonSelected)
				{
					m_isPlayButtonSelected = false;
					m_isExitButtonSelected = true;
				}
				break;
			}
			case sf::Keyboard::Return:
			{
				m_isPlayButtonPressed = false;
				m_isExitButtonPressed = false;

				if (m_isPlayButtonSelected)
				{
					m_isPlayButtonPressed = true;
				}
				else
				{
					m_isExitButtonPressed = true;
				}

				break;
			}
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
		m_exitButton.setFillColor(sf::Color::White);
	}
	else
	{
		m_exitButton.setFillColor(sf::Color::Yellow);
		m_playButton.setFillColor(sf::Color::White);
	}

	if (m_isPlayButtonPressed)
	{
		m_context->m_states->Add(std::make_unique<GamePlayState>(m_context), true);
	}
	else if (m_isExitButtonPressed)
	{
		m_context->m_states->PopAll();
	}
}