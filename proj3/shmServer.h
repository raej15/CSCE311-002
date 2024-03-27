// Copyright 2024 Rae Jones

#ifndef PROJ3_SHMSERVER_H_
#define PROJ3_SHMSERVER_H_

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sysinfo.h> // change back
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <sys/unistd.h>  // Unix standard header (ftruncate)

#define BUFFER_SIZE 25000
#define SHMPATH "raecool1"

#define SEM_SERVER "/sem_server"
#define SEM_CLIENT "/sem_client"

struct shmbuf {
    char buf[BUFFER_SIZE];
};

void quit();
#endif  // PROJ3_SHMSERVER_H_
