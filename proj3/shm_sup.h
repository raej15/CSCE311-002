// Copyright 2023 LittleCube
//

#ifndef PROJ3_SHM_SUP_H_
#define PROJ3_SHM_SUP_H_

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sysctl.h> // change back
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define BUFFER_SIZE 0x400
#define SHMPATH "csvshmem"

#define SEM_SERVER "/sem_server"
#define SEM_CLIENT "/sem_client"

struct shmbuf {
    char buf[BUFFER_SIZE];
};

void quit();
#endif  // PROJ3_SHM_SUP_H_