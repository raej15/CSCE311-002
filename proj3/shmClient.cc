// Copyright 2024 Rae Jones
#include <proj3/shmClient.h>
#include <proj3/shmConsts.h>
#include <proj3/shmStore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/un.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <iomanip>

typedef struct threadVars {
    int id;
    int lines;
    double tParSum;
} threadVars;

struct shmbuf *shmp;
struct shared_mem_struct::Store *store_;


std::vector<std::vector<std::string>> global[4];
std::vector<std::vector<std::string>> motherVect0;
std::vector<std::vector<std::string>> motherVect1;
std::vector<std::vector<std::string>> motherVect2;
std::vector<std::vector<std::string>> motherVect3;
double partialSums[4];
double finalSum = 0;

double add(double a, double b) {
    return a + b;
}
double subtract(double a, double b) {
    return a - b;
}
double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    return a / b;
}

//  finds and performs all the addition and subtraction operations in vector
std::vector<std::string> addSub(std::vector<std::string> eqn) {
    std::vector<std::string> result = eqn;
    auto itr = result.begin();
    while (itr != result.end()) {
        if (*itr == "+" || *itr == "-") {
            //  get the two surrounding floats
            double a = std::stod(*(itr - 1));
            double b = std::stod(*(itr + 1));

            if (*itr == "+") {
                //  perform addition, replace "+" with result
                *itr = std::to_string(add(a, b));
            } else {
                //  perform subtraction, replace "-" with result
                *itr = std::to_string(subtract(a, b));
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

//  finds and performs all the multiplication and division operations in vector
std::vector<std::string> multDiv(std::vector<std::string> eqn) {
    std::vector<std::string> result = eqn;
    auto itr = result.begin();
    while (itr != result.end()) {
        if (*itr == "x" || *itr == "/") {
            //  get the two surrounding floats
            double a = std::stod(*(itr - 1));
            double b = std::stod(*(itr + 1));

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

std::string run(std::vector<std::string> eqn) {
    std::vector<std::string> MDVect = multDiv(eqn);    //  MD of pemdas done
    std::vector<std::string> newVect = addSub(MDVect);  //  AS of pemdas done

    // print the resulting vector
    std::stringstream ss;
    for (auto itr = newVect.begin(); itr != newVect.end(); ++itr) {
        ss << *itr << " ";
    }

    std::string answer = newVect.at(0);
    return answer;
}

std::vector<std::string> loadData(std::string str) {
    std::vector<std::string> data;
    std::istringstream s(str);
    std::string line;
    int count = 0;
    while (std::getline(s, line)) {
        if (line == "INVALID FILE") {
            break;
        }
        data.push_back(line);
        std::cout << line << std::endl;
        count++;
    }
    return data;
}

std::vector<std::string> parseArgs(std::string eqn) {
    std::vector<std::string> parsedEqn;
    std::istringstream ss(eqn);
    std::string element;
    while (ss >> element) {
        parsedEqn.push_back(element);
    }
    return parsedEqn;
}

void fillGlobals(std::vector<std::string> data) {
    int dataSize = data.size();
    int counter = 0;
    for (int i = 0; i < dataSize; i++) {
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
}

void *threadSum(void *arg) {
    threadVars *tv = reinterpret_cast<threadVars *>(arg);
    int id = tv->id;
    double tParSum;
    std::vector<std::vector<std::string>> currVect = global[id];
    int currVectSize = currVect.size();
    for (int i = 0; i < currVectSize; i++) {
        std::string pSum = run(currVect[i]);
        tParSum = tParSum + std::stod(pSum);
    }
    tv->tParSum = tParSum;
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    // make sure shared memory does not already exist
    shm_unlink(SHMPATH);

    // open existing semaphores located on server
    sem_t *sem1 = sem_open(SEM_SERVER, 0);
    sem_t *sem2 = sem_open(SEM_CLIENT, 0);

    // STEP 1: create shared memory
    int shm_fd = shm_open(SHMPATH, O_CREAT | O_EXCL | O_RDWR,
                         S_IRUSR | S_IWUSR);
    ::ftruncate(shm_fd, sizeof(struct shmbuf));
    printf("SHARED MEMORY ALLOCATED: %ld BYTES\n", sizeof(struct shmbuf));

    // // map shared memory
    // shmp = reinterpret_cast<shmbuf *>(mmap(0,
    //                       sizeof(*shmp),
    //                       PROT_READ | PROT_WRITE,
    //                       MAP_SHARED,
    //                       shmfd,
    //                       0));

    // if (shmp == MAP_FAILED) {
    //     fprintf(stderr, "error mapping memory\n");
    //     return -1;
    // }

        const int kProt = PROT_READ | PROT_WRITE;
        store_ = static_cast< shared_mem_struct::Store*>(
            ::mmap(nullptr, shared_mem_struct::kCols, kProt, MAP_SHARED, shm_fd, 0));

        if (store_ == MAP_FAILED)
        {
            std::cerr << ::strerror(errno) << std::endl;

            ::exit(errno);
        }

        //store_->lens[0] = shared_mem_struct::kCols;  // set store's buffer size

        char read_buffer[shared_mem_struct::kCols];

    while (sem1 == 0) {
    }

    // notify server that shared memory is created
    sem_post(sem2);

    // wait for server to be ready to read
    sem_wait(sem1);

    // read_buffer[sizeof(read_buffer[0])-1][sizeof(read_buffer[1])-1]=0;

    // STEP 2: load the path into shared memory
    snprintf(store_->buf[0], BUFFER_SIZE, "%s", argv[1]);

    // notify server that string is ready to read
    sem_post(sem2);

    // wait for server to be ready to write
    sem_wait(sem1);

    // read string from shared memory
    for (int i = 0; i < 4; i++) {
        snprintf(read_buffer, shared_mem_struct::kCols, "%s", store_->buf[i]);
        std::cout << read_buffer << std::endl;
    }
    // // parse data from server
    // std::vector<std::string> data = loadData(read_buffer);
    // if (data.size() == 0) {
    //     std::cout << "INVALID FILE" << std::endl;
    //     shm_fd = shm_unlink(SHMPATH);
    //     exit(0);
    // }
    // fillGlobals(data);

    // // STEP 3: create threads
    // pthread_t threads[4];
    // threadVars tv[4];

    // for (int i = 0; i < 4; i++) {
    //     tv[i].id = i;
    //     tv[i].lines = global[i].size();
    //     pthread_create(&threads[i], NULL, threadSum,
    //                    reinterpret_cast<void *>(&(tv[i])));
    // }

    // std::cout << "THREADS CREATED" << std::endl;
    // for (int i = 0; i < 4; i++) {
    //     pthread_join(threads[i], NULL);
    // }

    // // STEP 4: print thread results
    // for (int i = 0; i < 4; i++) {
    //     std::cout << std::setprecision(15) << std::noshowpoint <<  "THREAD "
    //               << tv[i].id << ":  " << tv[i].lines << " LINES, "
    //               << tv[i].tParSum << std::endl;
    //     finalSum = finalSum + tv[i].tParSum;
    // }

    // std::cout << std::setprecision(15) << std::noshowpoint
    //           << "SUM: " << finalSum << std::endl;

    // STEP 5: destroy shared memory
    shm_fd = shm_unlink(SHMPATH);

    // STEP 6: exits
    exit(0);
}
