// Copyright 2024 Rae Jones
#include <proj3/shmServer.h>
#include <proj3/shmConsts.h>
#include <sys/sysinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

//struct Store *shmp;
struct shared_mem_struct::Store *store_;
//struct smhbuf *shmp;

std::vector<std::string> loadData(std::string fileName) {
    std::ifstream currFile(fileName.c_str());
    // std::ifstream currFile;
    // currFile.open(fileName.c_str());
    std::vector<std::string> data;
    std::string line;
    if (currFile.is_open()) {
        while (getline(currFile, line)) {
            data.push_back(line);
            // std::cout << line << std::endl;
        }
        currFile.close();
        std::cout << "\tFILE CLOSED" << std::endl;
    } else {
        std::cout << "\tINVALID FILE" << std::endl;
        // will send message to client via the shared memory
        data.push_back("INVALID FILE");
        return data;
    }

    return data;
}

std::string clientEqns(std::vector<std::string> data) {
    std::string finalStrng = "";
    int argSize = data.size();
    for (int i = 0; i < argSize; i++) {
        std::string eqn = data.at(i);
        finalStrng.append(eqn);
        finalStrng.append("\n");
    }
    return finalStrng;
}

int main(int argc, char **argv) {
    std::string path;
    std::vector<std::string> data;

    // STEP 1: write SERVER STARTED to stdout
    std::cout << "SERVER STARTED" << std::endl;

    // unlinking any pre-existing semaphores
    sem_unlink(SEM_SERVER);
    sem_unlink(SEM_CLIENT);
    while (true) {
        // create semaphores
        sem_t *sem1 = sem_open(SEM_SERVER, O_CREAT, 0660, 0);
        sem_t *sem2 = sem_open(SEM_CLIENT, O_CREAT, 0660, 0);
    
        // STEP 2: wait for client to open shared memory
        sem_wait(sem2);
        std::cout << "CLIENT REQUEST RECIEVED" << std::endl;

        // STEP 3: open shared memory
        int shm_fd = shm_open(SHMPATH, O_RDWR, 0);
        std::cout << "\tMEMORY OPEN" << std::endl;

        // get copy of mapped mem
        const int kProt = PROT_READ | PROT_WRITE;
        store_ = static_cast< shared_mem_struct::Store*>(
            ::mmap(nullptr, shared_mem_struct::kCols, kProt, MAP_SHARED, shm_fd, 0));

        if (store_ == MAP_FAILED)
        {
            std::cerr << ::strerror(errno) << std::endl;

            ::exit(errno);
        }

        char read_buffer[shared_mem_struct::kCols];

        // ready to read from client
        sem_post(sem1);

        // wait for client to finish writing
        sem_wait(sem2);

        // read path from shared memory
        snprintf(read_buffer, shared_mem_struct::kCols, "%s", store_->buf[0]);

        // STEP 4: open file from shared memory
        std::cout << "\tOPENING: " << read_buffer << std::endl;
        path = std::string(read_buffer);

        // INVALID FILE case is handled within loadData function
        // FILE CLOSED is handled within loadData function
        data = loadData(path);
        std::string eqnstr = clientEqns(data);

        // ready to write file to client
        sem_post(sem1);
        
        // write equations to shared memory
        snprintf(store_->buf[0], shared_mem_struct::kCols, "%s", eqnstr.c_str());

        // CLOSING SHARED MEMORY
        shm_fd = shm_unlink(SHMPATH);
        std::cout << "\tMEMORY CLOSED\n" << std::endl;
    }
}
