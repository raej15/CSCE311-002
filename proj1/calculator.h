#ifndef PROJ1_CALCULATOR_H_
#define PROJ1_CALCULATOR_H_
#include <string>
#include <iostream>
#include <vector>

float add(float a, float b);
float subtract(float a, float b);
float multiply(float a, float b);
float divide(float a, float b);
std::vector<std::string> addSub(std::vector<std::string> eqn);
std::vector<std::string> multDiv(std::vector<std::string> eqn);
void run(std::vector<std::string>);

#endif  // PROJ1_CALCULATE_H_