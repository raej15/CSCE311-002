/* pshm_ucase_send.c
Licensed under GNU General Public License v2 or later.
*/

#include <string.h>
#include <proj3/myClient.h>
#include <proj3/shm_store.h>

int main(int argc, char *argv[])
{
    int fd;
    char *shmpath, *string;
    size_t len;
    struct shmbuf *shmp;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s /shm-path string\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    shmpath = argv[1];
    string = argv[2];
    len = strlen(string);

    if (len > BUF_SIZE)
    {
        fprintf(stderr, "String is too long\n");
        exit(EXIT_FAILURE);
    }

    /* Open the existing shared memory object and map it
       into the caller's address space. */

    fd = shm_open(shmpath, O_RDWR, 0);
    if (fd == -1)
        errExit("shm_open");

    const int kProt = PROT_READ | PROT_WRITE;
    shmp = static_cast<SharedMemoryStore<kSharedMemSize> *>(
        ::mmap(nullptr, kSharedMemSize, kProt, MAP_SHARED, fd, 0));
    if (shmp == MAP_FAILED)
        errExit("mmap");

    /* Copy data into the shared memory object. */

    shmp->cnt = len;
    memcpy(&shmp->buf, string, len);

    /* Tell peer that it can now access shared memory. */

    if (sem_post(&shmp->sem1) == -1)
        errExit("sem_post");

    /* Wait until peer says that it has finished accessing
       the shared memory. */

    if (sem_wait(&shmp->sem2) == -1)
        errExit("sem_wait");

    /* Write modified data in shared memory to standard output. */

    write(STDOUT_FILENO, &shmp->buf, len);
    write(STDOUT_FILENO, "\n", 1);

    exit(EXIT_SUCCESS);
}