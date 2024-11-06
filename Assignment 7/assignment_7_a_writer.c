#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define BUFFER_SIZE 1024

int main()
{
    int fd1, fd2;
    char buffer[BUFFER_SIZE];

    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    fd1 = open(FIFO1, O_WRONLY);
    fd2 = open(FIFO2, O_RDONLY);

    printf("Enter Sentences (Ctrl+D to end):\n");

    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL)
    {
        write(fd1, buffer, strlen(buffer));
    }

    write(fd1, "EOF", 3);

    while (1)
    {
        int n = read(fd2, buffer, BUFFER_SIZE);
        if (n <= 0) break;
        write(STDOUT_FILENO, buffer, n);
    }
    
    close(fd1);
    close(fd2);

    unlink(FIFO1);
    unlink(FIFO2);

    return 0;
}