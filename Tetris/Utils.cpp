#include "Utils.h"

// Definição da variável engine
std::default_random_engine engine{ static_cast<unsigned int>(
    std::chrono::system_clock::now().time_since_epoch().count()) };

// Definição das funções
int getRandomNumber(int max) {
    std::uniform_int_distribution<int> dist(0, max);
    return dist(engine);
}

int getRandomNumber(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}