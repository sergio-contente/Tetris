#ifndef UTILS_H
#define UTILS_H

//#define NOMINMAX
////#include <windows.h>

#include <random>
#include <chrono>

// Declara��o da vari�vel engine
extern std::default_random_engine engine;

// Declara��o das fun��es
extern int getRandomNumber(int max);
extern int getRandomNumber(int min, int max);
#endif


