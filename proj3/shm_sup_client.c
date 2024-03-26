// Copyright 2023 LittleCube
//

#include </acct/sej15/Desktop/CSCE311-002/proj3/shm_sup.h>  // change
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
void *print_message_function( void *ptr );

void *print_message_function( void *ptr )
{
     char *message;
     message = (char *) ptr;
     printf("%s \n", message);
}

struct shmbuf* shmp;

int main(int argc, char** argv) {
    //char path[] = argv[1];
    printf("%s\n",argv[1]);
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
    ftruncate(shmfd, sizeof(struct shmbuf));
    printf("SHARED MEMORY SIZE: %ld BYTES\n", sizeof(struct shmbuf)); // ISs this done correctly??

    // map shared memory
    shmp = mmap(NULL,
                sizeof(*shmp),
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                shmfd,
                0);

    if (shmp == MAP_FAILED) {
        fprintf(stderr, "error mapping memory\n");
        return -1;
    }

    while (sem1 == 0) {}

    // notify server that shared memory is created
    sem_post(sem2);

    // prepare string to send
    //char sup_string[] = *argv[1];

    // wait for server to be ready to read
    sem_wait(sem1);

    //buf[sizeof(buff)-1]=0;
    // load the string into shared memory
    snprintf(shmp->buf, BUFFER_SIZE, "%s\n", argv[1]);
    
    //snprintf(shmp->buf, BUFFER_SIZE, "%s", argv[1]);

    // notify server that string is ready to read
    sem_post(sem2);
    // prepare string to send
    //char sup_string[] = *argv[1];

    // wait for server to be ready to read
    sem_wait(sem1);

    // load the string into shared memory
    snprintf(shmp->buf, BUFFER_SIZE, "%s\n", argv[2]);
    
    //snprintf(shmp->buf, BUFFER_SIZE, "%s", argv[1]);

    // notify server that string is ready to read
    sem_post(sem2);

    // wait for server to be ready to write
    sem_wait(sem3);

     pthread_t thread0, thread1, thread2, thread3;
     char *message0 = "Thread 0";
     char *message1 = "Thread 1";
     char *message2 = "Thread 2";
     char *message3 = "Thread 3";
     int  iret0, iret1, iret2, iret3;

    /* Create independent threads each of which will execute function */

     iret0 = pthread_create( &thread0, NULL, print_message_function, (void*) message0);
     iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);
     iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2);
     iret3 = pthread_create( &thread3, NULL, print_message_function, (void*) message3);
     
     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */
     
     pthread_join( thread0, NULL);
     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL); 
     pthread_join( thread3, NULL); 

     printf("THREAD 0: %d\n",iret0);
     printf("THREAD 1: %d\n",iret1);
     printf("THREAD 2: %d\n",iret2);
     printf("THREAD 3: %d\n",iret3);
     exit(0);
}
