#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SHM_SIZE 1024

int main()
{
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, SHM_UNLOCK, 0666);
    char *str = (char *)shmat(shmid, NULL, 0);

    printf("\nMessage Received From Client: %s\n", str);

    shmdt(str);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;   
}