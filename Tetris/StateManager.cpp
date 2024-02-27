#include "StateManager.h"

StateManager::StateManager() : m_running(true) {
	std::cout << "StateManager Initialized! " << std::endl;
}

StateManager::~StateManager() {
	std::cout << "StateManager Destroyed!" << std::endl;

	while (!states.empty()) {
		PopState();
	}
}
