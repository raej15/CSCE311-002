//  COPYRIGHT 2024 Rae Jones
#ifndef PROJ2_MYSERVER_H_
#define PROJ2_MYSERVER_H_
#define SOCKET_NAME "/tmp/9Lq7BNBnBycd6nxy.socket"  // get rid of this
#define BUFFER_SIZE 10000  // get rid of this

#include <iostream>
#include <string>
#include <vector>

float add(float a, float b);
float subtract(float a, float b);
float multiply(float a, float b);
float divide(float a, float b);
std::vector<std::string> addSub(std::vector<std::string> eqn);
std::vector<std::string> multDiv(std::vector<std::string> eqn);

#endif  // PROJ2_MYSERVER_H_
