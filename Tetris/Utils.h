#ifndef UTILS_H
#define UTILS_H

//#define NOMINMAX
////#include <windows.h>

#include <random>
#include <chrono>

// Declaração da variável engine
extern std::default_random_engine engine;

// Declaração das funções
extern int getRandomNumber(int max);
extern int getRandomNumber(int min, int max);
#endif


