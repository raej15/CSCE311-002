// Copyright 2024 Rae Jones
#include <proj3/shmServer.h> //change
#include <sys/sysinfo.h>
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
#include <signal.h>

struct shmbuf* shmp;

float add(float a, float b) {
    return a + b;
}
float subtract(float a, float b) {
    return a - b;
}
float multiply(float a, float b) {
    return a*b;
}

float divide(float a, float b) {
    return a/b;
}

//  finds and performs all the addition and subtraction operations in vector
std::vector<std::string> addSub(std::vector<std::string> eqn) {
    std::vector<std::string> result = eqn;
    auto itr = result.begin();
    while ( itr != result.end() ) {
        if (*itr == "+" || *itr == "-") {
            //  get the two surrounding floats
            float a = std::stof(*(itr-1));
            float b = std::stof(*(itr+1));

            if (*itr == "+") {
                //  perform addition, replace "+" with result
                *itr = std::to_string(add(a, b));
                std::cout << a << "+" << b << subtract(a, b);
            } else {
                //  perform subtraction, replace "-" with result
                *itr = std::to_string(subtract(a, b));
               std::cout << a << "-" << b << subtract(a, b);
            }

            //  remove the two surrounding floats
            itr = result.erase(itr-1);
            itr = result.erase(itr+1);
        } else {
            //  move to the next element
            ++itr;
        }
    }
    //  returns new vector
    return result;
}

//  finds and performs all the addition and subtraction operations in vector
std::vector<std::string> multDiv(std::vector<std::string> eqn) {
    std::vector<std::string> result = eqn;
    auto itr = result.begin();
    while (itr != result.end()) {
        if (*itr == "x" || *itr == "/") {
            //  get the two surrounding floats
            float a = std::stof(*(itr-1));
            float b = std::stof(*(itr+1));

            if (*itr == "/") {
                //  perform division, replace "/" with result
                *itr = std::to_string(divide(a, b));
            } else {
                //  perform multiplication, replace "x" with result
                *itr = std::to_string(multiply(a, b));
            }

            //  remove the two surrounding floats
            itr = result.erase(itr-1);
            itr = result.erase(itr+1);

        } else {
            //  move to the next element
            ++itr;
        }
    }

    //  returns new vector
    return result;
}

std::vector<std::string> loadData(std::string fileName) {
    //std::cout << "entered filename: " << fileName << std::endl;
    std::ifstream currFile2("dat/equations_691.txt"); // CHANGE!!!!
    std::ifstream currFile(fileName.c_str());
    //std::ifstream currFile;
    //currFile.open(fileName.c_str());
    std::vector<std::string> data;
    std::string line;
    if (currFile.is_open()) {
        while (getline(currFile, line)) {
        data.push_back(line);
        //std::cout << line << std::endl;
        }
        currFile.close();
       std::cout << "\tFILE CLOSED" << std::endl;
    } else {
        data.push_back("INVALID FILE");
      return data;
    }

    return data;
}

void parseArgs(std::vector<std::string> data,
    std::vector<std::string> argLines) {
    int argSize = argLines.size();
    for (int i = 0; i < argSize; i++) {
        std::vector<std::string> parsedEqn;
        int curr = stoi(argLines.at(i))-1;
        std::string eqn = data.at(curr);
        std::cout << "EQUATION: " << eqn << std::endl;

        std::istringstream ss(eqn);
        std::string element;
        while (ss >> element) {
            std::cout << element << std::endl;
            parsedEqn.push_back(element);
        }
        // return parsedEqn; // fix hoe
    }
    // return ;
}

std::string clientEqns(std::vector<std::string> data) {
    std::string finalStrng = "";
       int argSize = data.size();
    for (int i=0; i < argSize; i++) {
        std::string eqn = data.at(i);
        finalStrng.append(eqn);
        finalStrng.append("\n");
    }
    return finalStrng;
}
int main(int argc, char** argv) {
    std::string path;
    std::vector<std::string> data;
    std::cout<< "SERVER STARTED\n" << std::endl;
    
    // happy signal time (properly cleanup on terminate)
 //   signal(SIGTERM, quit);
//    signal(SIGINT, quit);

    // make sure semaphores do not already exist
    sem_unlink(SEM_SERVER);
    sem_unlink(SEM_CLIENT);

    // create previously nonexistent semaphores
    // sem_open returns -1 on fail, perhaps worth checking
    sem_t *sem1 = sem_open(SEM_SERVER, O_CREAT, 0660, 0);
    sem_t *sem2 = sem_open(SEM_CLIENT, O_CREAT, 0660, 0);
    sem_t *sem3 = sem_open(SEM_SERVER, O_CREAT, 0660, 0);


    // wait for client to open shared memory
    sem_wait(sem2);
    std::cout << "CLIENT REQUEST RECIEVED\n" << std::endl;

    // so does shm_open
    int shmfd = shm_open(SHMPATH, O_RDWR, 0);
    std::cout << "\tMEMORY OPEN\n" << std::endl;

    // map shared memory
    // BUFFER_SIZE is defined in shm_sup.h
    shmp = (shmbuf*)mmap(0,
                sizeof(*shmp),
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                shmfd,
                0);

    char read_buffer[BUFFER_SIZE];

    // ready to read from client
    sem_post(sem1);

    // wait for client to finish writing
    sem_wait(sem2);

    // read path from shared memory
    snprintf(read_buffer, BUFFER_SIZE, "%s", shmp->buf);

    // print client path from read_buffer
    std::cout<< "\tOPENING: " << read_buffer <<std::endl;
    path = std::string(read_buffer);
    data = loadData(path);
    std::string eqnstr = clientEqns(data);
    
    // ready to read from client
    sem_post(sem1);

    // wait for client to finish writing
    sem_wait(sem2);

    // read string from shared memory
    snprintf(read_buffer, BUFFER_SIZE, "%s", shmp->buf);

    // print client string from read_buffer
   // printf("LINES: %s (REMOVE)", read_buffer);
    
    // ready to write file to client
    sem_post(sem3);

    // writing file to client
    snprintf(shmp->buf, BUFFER_SIZE, "%s\n", eqnstr.c_str());

    //printf("SERVER: GOODBYE\n");
}

// I'm a happy signal boy
//void quit() {
//    shm_unlink(SHMPATH);
//    sem_unlink(SEM_SERVER);
//    sem_unlink(SEM_CLIENT);
//    exit(0);
//}
