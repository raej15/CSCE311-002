#ifndef PROJ3_MYSERVER_H_
#define PROJ3_MYSERVER_H_
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>  // POSIX shared memory map
#include <sys/unistd.h>  // Unix standard header (ftruncate)

#include <cassert>
#include <cerrno>
#include <csignal>
#include <cstddef>  // size_t

#include <fstream>
#include <iostream>
#include <string>

#define errExit(msg)        \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

#define BUF_SIZE 1024 /* Maximum size for exchanged string */

/* Define a structure that will be imposed on the shared
   memory object */

struct shmbuf
{
    sem_t sem1;         /* POSIX unnamed semaphore */
    sem_t sem2;         /* POSIX unnamed semaphore */
    size_t cnt;         /* Number of bytes used in 'buf' */
    char buf[BUF_SIZE]; /* Data being transferred */
};

class Consumer {
 public:
  Consumer(const char* kSHM_name, const char* shm_log_signal_name);


  // Unlinks shared memory for future use
  //
  ~Consumer();

  // Do forever: while there are logs in log, write to file
  //
  void Consume(const char log_name[]);

 private:
  static const std::size_t kBufferSize = 1024;
  static const std::size_t kSharedMemSize
    = sizeof(SharedMemoryStore<kBufferSize>);

  const char* shm_name_;

  SharedMemoryStore<kSharedMemSize> *store_;

  //wrappers::NamedSemaphore shm_log_signal_;
};

#endif // PROJ3_MYSERVER_H_