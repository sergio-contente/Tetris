#include "StateManager.h"

StateManager::StateManager() : m_running(true) {
}

StateManager::~StateManager() {
    while (!states.empty()) {
        delete states.back();
        states.pop_back();
    }
}

void StateManager::ChangeState(State* state) {
    while (!states.empty()) {
        states.back()->Cleanup();
        delete states.back();
        states.pop_back();
    }

    states.push_back(state);
    states.back()->Init();
}

void StateManager::PushState(State* state) {
    if (!states.empty()) {
        states.back()->Pause();
    }

    states.push_back(state);
    states.back()->Init();
}

void StateManager::PopState() {
    if (!states.empty()) {
        states.back()->Cleanup();
        delete states.back();
        states.pop_back();
    }

    if (!states.empty()) {
        states.back()->Resume();
    }
}

void StateManager::HandleEvents() {
    if (!states.empty()) {
        states.back()->HandleEvents();
    }
}

void StateManager::Update(sf::Time deltaTime) {
    if (!states.empty()) {
        states.back()->Update(deltaTime);
    }
}

void StateManager::Draw() {
    if (!states.empty()) {
        states.back()->Draw();
    }
}

bool StateManager::IsEmpty() const {
    return states.empty();
}
