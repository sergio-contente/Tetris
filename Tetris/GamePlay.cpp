#include "GamePlay.h"

GamePlay::GamePlay(std::shared_ptr<Context>& context) : m_context(context) {}
GamePlay::~GamePlay() {}

void GamePlay::Init() {
}
void GamePlay::Cleanup() {
}
void GamePlay::Pause() {
}
void GamePlay::Resume() {
}
void GamePlay::HandleEvents() {
}
void GamePlay::Update(sf::Time deltaTime) {
}
void GamePlay::Draw() {
}