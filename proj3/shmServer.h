// Copyright 2024 Rae Jones

#ifndef PROJ3_SHMSERVER_H_
#define PROJ3_SHMSERVER_H_

#pragma once

#include <proj3/shmConsts.h>


#define BUFFER_SIZE mem[1<<2][1<<19]
#define SHMPATH "raecool1"

#define SEM_SERVER "/sem_server"
#define SEM_CLIENT "/sem_client"

struct shmbuf {
    char buf[BUFFER_SIZE];
};

void quit();
#endif  // PROJ3_SHMSERVER_H_
