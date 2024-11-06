#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SHM_SIZE 1024

int main()
{
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    char *str = (char *)shmat(shmid, NULL, 0);

    printf("\nEnter Message To Be Written In Shared Memory:\n");
    fgets(str, SHM_SIZE, stdin);
    printf("\n");
    
    shmdt(str);

    return 0;
}