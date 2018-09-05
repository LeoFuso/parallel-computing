#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

void
ClientProcess(int [], int, int);

int
isPrime(int);

int
main(int argc, char *argv[])
{
    int size;
    int ShmID;
    int *ShmPTR;
    pid_t pid;
    int status;

    if (argc != 2)
    {
        printf("Use: %s #1\n", argv[0]);
        exit(1);
    }

    size = (int) strtol(argv[1], (char **) NULL, 10);

    if (size <= 0)
    {
        printf("*** Illegal argument error (server) ***\n");
        exit(1);
    }

    ShmID = shmget(IPC_PRIVATE, size * sizeof(int), IPC_CREAT | 0666);
    if (ShmID < 0)
    {
        printf("*** shmget error (server) ***\n");
        exit(1);
    }
    printf("Server has received a shared memory of %d integer(s)...\n", size);

    ShmPTR = (int *) shmat(ShmID, NULL, 0);
    if ((int) ShmPTR == -1)
    {
        printf("*** shmat error (server) ***\n");
        exit(1);
    }
    printf("Server has attached the shared memory...\n");

    /* Filling array */
    int n;
    for (n = 0; n < size; n++)
        ShmPTR[n] = n + 1;

    printf("Server has filled an array of size %d in shared memory...\n", size);

    int middle = size / 2;
    printf("Server is about to fork a child process...\n");

    pid = fork();
    if (pid < 0)
    {
        printf("*** fork error (server) ***\n");
        exit(1);
    }
    else if (pid == 0)
    {
        ClientProcess(ShmPTR, 0, middle);
        exit(0);
    }

    wait(&status);
    printf("Server has detected the completion of its child...\n");
    printf("Server is trying to find the prime number\n");

    for (n = middle + 1; n < size; n++)
        if (!isPrime(ShmPTR[n]))
            ShmPTR[n] = 0;


    printf("Prime numbers found: \n");
    for (n = middle + 1; n < size; n++)
        if (ShmPTR[n] != 0)
            printf("array[%d]: %d\n", n, ShmPTR[n]);

    shmdt((void *) ShmPTR);

    printf("Server has detached its shared memory...\n");
    shmctl(ShmID, IPC_RMID, NULL);
    printf("Server has removed its shared memory...\n");
    printf("Server exits...\n");
    exit(0);
}

void
ClientProcess(int SharedMem[], int start, int end)
{
    printf("   Client process started\n");
    printf("   Client found an array in shared memory\n");
    printf("   Array:\n");
    int n;
    for (n = start; n < end; n++)
        printf("   array[%d]: %d\n", n, SharedMem[n]);

    for (n = start; n < end; n++)
        if (!isPrime(SharedMem[n]))
            SharedMem[n] = 0;

    printf("   Client is trying to find the prime number\n");
    printf("   Prime numbers found: \n");
    for (n = start; n < end; n++)
        if (SharedMem[n] != 0)
            printf("   array[%d]: %d\n", n, SharedMem[n]);
    printf("   Client is about to exit\n");
}

int
isPrime(int n)
{
    unsigned int i;
    for (i = 2; i <= n / 2; i++)
    {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

