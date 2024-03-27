// Copyright 2024 Rae Jones

#ifndef PROJ3_SHMCONSTS_H_
#define PROJ3_SHMCONSTS_H_

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <sys/unistd.h>  // Unix standard header (ftruncate)

// char mem[1<<2][1<<19];

#define BUFFER_SIZE 524288  // change
#define SHMPATH "raecool"

#define SEM_SERVER "/sem_server23231"
#define SEM_CLIENT "/sem_client23231"

struct shmbuf {
    char buf[BUFFER_SIZE];
};

void quit();
#endif  // PROJ3_SHMCONSTS_H_
