#pragma once


#include <random>

#include <chrono>
std::default_random_engine engine{ static_cast<unsigned int>(
        std::chrono::system_clock::now().time_since_epoch().count()) };


int getRandomNumber(int max) {
    std::uniform_int_distribution<int> int_distribution(max);
    return int_distribution(engine);
}

int getRandomNumber(int min, int max) {
    std::uniform_int_distribution<int> int_distribution(min, max);
    return int_distribution(engine);
}

