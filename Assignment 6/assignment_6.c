#include <stdio.h>

#define MAX_PAGES 30
#define MAX_FRAMES 10

int search(int page, int frames[], int frame_size)
{
    for (int i = 0; i < frame_size; i++)
        if (page == frames[i])
            return i;
    return -1;
}

void printFrames(int frames[], int frame_size)
{
    printf("\nFrames: ");
    for (int i = 0; i < frame_size; i++)
    {
        if (frames[i] == -1)
            printf("[ ] ");
        else
            printf("[%d] ", frames[i]);
    }
}

int fifo(int pages[], int n, int frame_size)
{
    int frames[MAX_FRAMES];
    int page_faults = 0;
    int frame_index = 0;

    for (int i = 0; i < frame_size; i++)
        frames[i] = -1;

    printf("\nFIFO Page Replacement Algorithm\n");

    for (int i = 0; i < n; i++)
    {
        printf("\n\nReference Page %d: ", pages[i]);
        if (search(pages[i], frames, frame_size) == -1)
        {
            frames[frame_index] = pages[i];
            frame_index = (frame_index + 1) % frame_size;
            page_faults++;
            printf("Page Fault!");
        }
        else
            printf("Page Hit!");
        printFrames(frames, frame_size);
    }

    return page_faults;
}

int lru(int pages[], int n, int frame_size)
{
    int frames[MAX_FRAMES] = {-1};
    int page_faults = 0;
    int counter[MAX_FRAMES] = {0};

    for (int i = 0; i < frame_size; i++)
        frames[i] = -1;

    printf("\nLRU Page Replacement Algorithm");

    for (int i = 0; i < n; i++)
    {
        printf("\n\nReference Page %d: ", pages[i]);
        int page = pages[i];
        int pos = search(page, frames, frame_size);

        if (pos == -1)
        {
            int min_counter = counter[0];
            int replace_pos = 0;

            for (int j = 1; j < frame_size; j++)
            {
                if (counter[j] < min_counter)
                {
                    min_counter = counter[j];
                    replace_pos = j;
                }
            }
            
            frames[replace_pos] = page;
            counter[replace_pos] = i + 1;
            page_faults++;
            printf("Page Fault!");
        }
        else
        {
            counter[pos] = i + 1;
            printf("Page Hit!");
        }
        printFrames(frames, frame_size);
    }
    
    return page_faults;
}

int optimal(int pages[], int n, int frame_size)
{
    int frames[MAX_FRAMES] = {-1};
    int page_faults = 0;

    for (int i = 0; i < frame_size; i++)
        frames[i] = -1;

    printf("\nOptimal Page Replacement Algorithm");

    for (int i = 0; i < n; i++)
    {
        printf("\n\nReference Page %d: ", pages[i]);
        int page = pages[i];
        int pos = search(page, frames, frame_size);

        if (pos == -1)
        {
            int farthest = -1;
            int replace_pos = 0;
            for (int j = 0; j < frame_size; j++)
            {
                int k;
                for (k = i + 1; k < n; k++)
                {
                    if (frames[j] == pages[k])
                    {
                        if (k > farthest)
                        {
                            farthest = k;
                            replace_pos = j;
                        }
                        break;
                    }
                }
                if (k == n)
                {
                    replace_pos = j;
                    break;
                }
            }
            frames[replace_pos] = page;
            page_faults++;
            printf("Page Fault!");
        }
        else
            printf("Page Hit!");
        
        printFrames(frames, frame_size);
    }
    
    return page_faults;
}

int main()
{
    int pages[MAX_PAGES];
    int n, frame_size, choice;

    printf("\nEnter Number Of Pages: ");
    scanf("%d", &n);
    printf("Enter Page Reference String: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);
    printf("Enter Number Of Frames: ");
    scanf("%d", &frame_size);

    do
    {
        printf("\n--- Page Replacement Algorithms ---\n");
        printf("1. FIFO\n");
        printf("2. LRU\n");
        printf("3. Optimal\n");
        printf("4. Exit\n");
        printf("\nEnter Your Choice: ");
        scanf("%d", &choice);

        if (choice == 1)
            printf("\n\nFIFO Page Faults: %d\n", fifo(pages, n, frame_size));
        else if (choice == 2)
            printf("\n\nLRU Page Faults: %d\n", lru(pages, n, frame_size));
        else if (choice == 3)
            printf("\n\nOptimal Page Faults: %d\n", optimal(pages, n, frame_size));
        
        else if (choice == 4)
            printf("\nExiting Program....\n");
        else
            printf("\nEnter Valid Choice!\n");

    } while (choice != 4);

    return 0;
}