// Copyright 2024 Rae Jones
#include <proj3/shmClient.h>
#include <proj3/shmConsts.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

std::vector<std::vector<std::string>> global[4];
long partialSums[4];
long finalSum = 0;

long add(long a, long b) {
    return a + b;
}
long subtract(long a, long b) {
    return a - b;
}
long multiply(long a, long b) {
    return a * b;
}

long divide(long a, long b) {
    return a / b;
}

//  finds and performs all the addition and subtraction operations in vector
std::vector<std::string> addSub(std::vector<std::string> eqn) {
    std::vector<std::string> result = eqn;
    auto itr = result.begin();
    while (itr != result.end()) {
        if (*itr == "+" || *itr == "-") {
            //  get the two surrounding floats
            long a = std::stol(*(itr - 1));
            long b = std::stol(*(itr + 1));

            if (*itr == "+") {
                //  perform addition, replace "+" with result
                *itr = std::to_string(add(a, b));
                // std::cout << a << "+" << b << subtract(a, b);
            } else {
                //  perform subtraction, replace "-" with result
                *itr = std::to_string(subtract(a, b));
                // std::cout << a << "-" << b << subtract(a, b);
            }

            //  remove the two surrounding floats
            itr = result.erase(itr - 1);
            itr = result.erase(itr + 1);
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
            long a = std::stol(*(itr - 1));
            long b = std::stol(*(itr + 1));

            if (*itr == "/") {
                //  perform division, replace "/" with result
                *itr = std::to_string(divide(a, b));
            } else {
                //  perform multiplication, replace "x" with result
                *itr = std::to_string(multiply(a, b));
            }

            //  remove the two surrounding floats
            itr = result.erase(itr - 1);
            itr = result.erase(itr + 1);
        } else {
            //  move to the next element
            ++itr;
        }
    }

    //  returns new vector
    return result;
}

//  needs to accept negs
std::string run(std::vector<std::string> eqn) {
    std::vector<std::string> MDVect = multDiv(eqn);  //  MD of pemdas done
    std::vector<std::string> newVect = addSub(MDVect);  //  AS of pemdas done

    // print the resulting vector
    std::stringstream ss;
    for (auto itr = newVect.begin(); itr != newVect.end(); ++itr) {
        // std::cout << *itr << std::endl;
        ss << *itr << " ";
    }
    // std::cout << newVect.at(0) << std::endl;

    std::string answer = newVect.at(0);
    return answer;
}

std::vector<std::string> loadData(std::string str) {
    std::vector<std::string> data;
    std::istringstream s(str);
    std::string line;
    int count = 0;
    while (std::getline(s, line)) {
        data.push_back(line);
        count++;
    }
    return data;
}

std::vector<std::string> parseArgs(std::string eqn) {
    std::vector<std::string> parsedEqn;
    std::istringstream ss(eqn);
    std::string element;
    while (ss >> element) {
        // std::cout << element << std::endl;
        parsedEqn.push_back(element);
    }
    return parsedEqn;
}

std::string clientEqns(std::vector<std::string> data,
                       std::vector<std::string> argLines) {
    std::string finalStrng = "";
    int argSize = argLines.size();
    for (int i = 0; i < argSize; i++) {
        int curr = stoi(argLines.at(i)) - 1;
        std::string eqn = data.at(curr);
        // std::cout << "EQUATION: " << eqn << std::endl;

        // std::istringstream ss(eqn);
        // std::string element;
        // while (ss >> element)
        // {
        // /    std::cout << element << std::endl;
        //     parsedEqn.push_back(element);
        // }
        // return parsedEqn; // fix hoe
        finalStrng.append("line ");
        finalStrng.append(std::to_string(curr + 1));
        finalStrng.append(": ");
        finalStrng.append(eqn);
        finalStrng.append("\n");
    }
    return finalStrng;
}

void *threadSum(void *id) {
    long* thread_ids;
    sleep(1);
    thread_ids = (long*) id;
    std::vector<std::string> sum;
    std::vector<std::vector<std::string>> currVect = global[*thread_ids];
    long tSum = 0;
    int currVectSize = currVect.size();
    for (int i = 0; i < currVectSize; i++) {
        std::string pSum = run(currVect[i]);
        tSum = tSum + std::stol(pSum);
    }

    partialSums[*thread_ids] = tSum;
    std::cout << "THREAD " << *thread_ids << ": " << tSum << std::endl;
    return 0;
}
struct shmbuf *shmp;

int main(int argc, char **argv) {
    // make sure shared memory does not already exist
    shm_unlink(SHMPATH);

    // open existing semaphores located on server
    sem_t *sem1 = sem_open(SEM_SERVER, 0);
    sem_t *sem2 = sem_open(SEM_CLIENT, 0);
    sem_t *sem3 = sem_open(SEM_SERVER, 0);

    // create previously uninstantiated shared memory
    int shmfd = shm_open(SHMPATH, O_CREAT | O_EXCL | O_RDWR,
                         S_IRUSR | S_IWUSR);

    // Truncate the memory (VERY IMPORTANT, will get a Bus Error otherwise)
    ::ftruncate(shmfd, sizeof(struct shmbuf));
    printf("SHARED MEMORY SIZE: %ld BYTES\n", sizeof(struct shmbuf));

    // map shared memory
    shmp = (shmbuf *)mmap(0,
                          sizeof(*shmp),
                          PROT_READ | PROT_WRITE,
                          MAP_SHARED,
                          shmfd,
                          0);

    if (shmp == MAP_FAILED) {
        fprintf(stderr, "error mapping memory\n");
        return -1;
    }
    char read_buffer[BUFFER_SIZE];

    while (sem1 == 0) {
    }

    // notify server that shared memory is created
    sem_post(sem2);

    // prepare string to send
    // char sup_string[] = *argv[1];

    // wait for server to be ready to read
    sem_wait(sem1);

    // buf[sizeof(buff)-1]=0;
    //  load the string into shared memory
    snprintf(shmp->buf, BUFFER_SIZE, "%s", argv[1]);

    // snprintf(shmp->buf, BUFFER_SIZE, "%s", argv[1]);

    // notify server that string is ready to read
    sem_post(sem2);

    // wait for server to be ready to read
    sem_wait(sem1);

    // load the string into shared memory
    snprintf(shmp->buf, BUFFER_SIZE, "\n%s\n", argv[2]);

    // snprintf(shmp->buf, BUFFER_SIZE, "%s", argv[1]);

    // notify server that string is ready to read
    sem_post(sem2);

    // wait for server to be ready to write
    sem_wait(sem3);

    // read string from shared memory
    snprintf(read_buffer, BUFFER_SIZE, "%s", shmp->buf);

    // print client string from read_buffer
    // printf("%s", read_buffer);

    // std::string data = std::string(read_buffer);

    std::vector<std::vector<std::string>> motherVect0;
    std::vector<std::vector<std::string>> motherVect1;
    std::vector<std::vector<std::string>> motherVect2;
    std::vector<std::vector<std::string>> motherVect3;

    std::vector<std::string> data = loadData(read_buffer);
    int dataSize = data.size();
    int counter = 0;
    for (int i = 0; i < dataSize-1; i++) {
        std::string curr = data.at(i);
        std::vector<std::string> finalEqn = parseArgs(curr);
        if (counter == 0) {
            motherVect0.push_back(finalEqn);
        } else if (counter == 1) {
            motherVect1.push_back(finalEqn);
        } else if (counter == 2) {
            motherVect2.push_back(finalEqn);
        } else {
            motherVect3.push_back(finalEqn);
        }
        if (counter == 3) {
            counter = 0;
        } else {
            counter++;
        }
    }    
    global[0] = motherVect0;
    global[1] = motherVect1;
    global[2] = motherVect2;
    global[3] = motherVect3;
    // std::cout << "yolo" << std::endl;

    // for (int i=0; i < motherVect0.size(); i++) {
    //      for (int j=0; j<motherVect0[i].size(); j++) {
    //         //std::cout << motherVect0[i][j] << " ";
    //     }
    //     //std::cout << std::endl;
    // }
    pthread_t threads[4];  // creates 4 threads
    long thread_ids[4] = {0, 1 , 2, 3};  // creates 4 thread_data structs

    for (int i = 0; i < 4; i++) {
        int tr = pthread_create(&threads[i], NULL, threadSum,
                                  (void*) (&(thread_ids[i])));
    }
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < 4; i++) {
        finalSum = finalSum + partialSums[i];
    }

    std::cout << "SUM: " << finalSum << std::endl;

    exit(0);
}
