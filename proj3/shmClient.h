// Copyright 2024 Rae Jones
#ifndef PROJ3_SHMCLIENT_H_
#define PROJ3_SHMCLIENT_H_
#include <proj3/shmConsts.h>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> addSub(std::vector<std::string> eqn);
std::vector<std::string> multDiv(std::vector<std::string> eqn);
std::string run(std::vector<std::string> eqn);
std::vector<std::string> loadData(std::string str);
std::vector<std::string> parseArgs(std::string eqn);
void fillGlobals(std::vector<std::string> data);
void *threadSum(void *arg);

#endif  // PROJ3_SHMCLIENT_H_
