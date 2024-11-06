#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define BUFFER_SIZE 1024
#define OUTPUT_FILE "stats.txt"

int main()
{
    int fd1, fd2;
    char buffer[BUFFER_SIZE];
    int chars = 0, words = 0, lines = 0;
    int in_word = 0;
    FILE *fp;

    fd1 = open(FIFO1, O_RDONLY);
    fd2 = open(FIFO2, O_WRONLY);

    while (1)
    {
        int n = read(fd1, buffer, BUFFER_SIZE);
        if (n <= 0) break;

        if (strncmp(buffer, "EOF", 3) == 0) break;

        for (int i = 0; i < n; i++)
        {
            chars++;

            if (isspace(buffer[i]))
            {
                if (in_word)
                {
                    words++;
                    in_word = 0;
                }
                if (buffer[i] == '\n')
                {
                    lines++;
                }
            }
            else
                in_word = 1;
        }
    }

    if (in_word) words++;

    fp = fopen(OUTPUT_FILE, "w");
    fprintf(fp, "Characters: %d\nWords: %d\nLines: %d\n", chars, words, lines);
    fclose(fp);
    
    fp = fopen(OUTPUT_FILE, "r");
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL)
    {
        write(fd2, buffer, strlen(buffer));
    }
    fclose(fp);

    close(fd1);
    close(fd2);

    return 0;
}