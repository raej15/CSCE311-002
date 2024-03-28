// Copyright 2024 Rae Jones

#ifndef PROJ3_SHMSERVER_H_
#define PROJ3_SHMSERVER_H_

#include <proj3/shmConsts.h>
#include <proj3/shmStore.h>
#include <iostream>
#include <string>
#include <vector>

struct shmbuf *shmp;
std::vector<std::string> loadData(std::string fileName);
std::string clientEqns(std::vector<std::string> data);


#endif  // PROJ3_SHMSERVER_H_
