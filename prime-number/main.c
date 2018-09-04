#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

void
ClientProcess (int [], int);

int
main (int argc, char *argv[])
{
	int size;
	int ShmID;
	int *ShmPTR;
	pid_t pid;
	int status;

	if (argc != 2)
	{
		printf ("Use: %s #1\n", argv[0]);
		exit (1);
	}

	size = (int) strtol (argv[1], (char **) NULL, 10);

	if (size <= 0)
	{
		printf ("*** Illegal argument error (server) ***\n");
		exit (1);
	}

	ShmID = shmget (IPC_PRIVATE, size * sizeof (int), IPC_CREAT | 0666);
	if (ShmID < 0)
	{
		printf ("*** shmget error (server) ***\n");
		exit (1);
	}
	printf ("Server has received a shared memory of %d integer(s)...\n", size);

	ShmPTR = (int *) shmat (ShmID, NULL, 0);
	if ((int) ShmPTR == -1)
	{
		printf ("*** shmat error (server) ***\n");
		exit (1);
	}
	printf ("Server has attached the shared memory...\n");

	unsigned int n;
	for (n = 0; n < size; n++) ShmPTR[n] = n + 1;

	printf ("Server has filled an array of size %d in shared memory...\n", size);

	printf ("Server is about to fork a child process...\n");
	pid = fork ();
	if (pid < 0)
	{
		printf ("*** fork error (server) ***\n");
		exit (1);
	}
	else if (pid == 0)
	{
		ClientProcess (ShmPTR, size / 2);
		exit (0);
	}

	wait (&status);
	printf ("Server has detected the completion of its child...\n");
	printf ("Server is about to manipulate the returned data\n");
	ShmPTR[0] *= 4;
	printf ("Current value in shared memory: %d\n",
	        ShmPTR[0]);
	shmdt ((void *) ShmPTR);
	printf ("Server has detached its shared memory...\n");
	shmctl (ShmID, IPC_RMID, NULL);
	printf ("Server has removed its shared memory...\n");
	printf ("Server exits...\n");
	exit (0);
}

void
ClientProcess (int SharedMem[], int size)
{
	printf ("   Client process started\n");
	printf ("   Client found %d in shared memory\n",
	        SharedMem[0]);
	SharedMem[0] += 2;
	printf ("   Client is about to manipulate the data\n");
	printf ("   Current value in shared memory: %d\n",
	        SharedMem[0]);
	printf ("   Client is about to exit\n");
}

