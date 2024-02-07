#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define TERM_TAIL_CLEAR         printf("\033[2J");
#define TERM_HEAD_CLEAR         printf("\033[2J\033[0;0H");
#define TERM_CORS_CLEAR         printf("\033[K");
#define TERM_CORS_MOVE(y, x)    printf("\033[%d;%dH", y, x);
#define TERM_CORS_RIGHT(n)      printf("\033[%dC", n);

static unsigned long long totol_length = 0;

static time_t time_start;

static int pkt_num = 0;

int auto_string_create(char *string, int max_length, int *length)
{
    int i   = 0;
    int len = 0;
    int dim = 0;

    len = rand() % max_length;

    for (i = 0; i < len; ++i)
    {
        dim = rand() % 0x7E;

        if (dim <= 0x20)
        {
            dim += 0x20;
        }

        string[i] = dim;
    }

    string[i] = 0;
    *length = i;

    return 0;
}

int auto_string_show(char *string, int length)
{
    int i = 0;

    struct winsize size;
    time_t timep;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    TERM_HEAD_CLEAR;

    TERM_CORS_MOVE(2, 0);

    for (i = 0; i < size.ws_col; ++i)
    {
        printf("\033[31m-\033[0m");
    }
    printf("\n");

    printf("\033[32;1mSTRING CONTENT:\033[0m\n\n");

    printf("\033[33m%s\033[0m\n", string);

    TERM_CORS_MOVE(size.ws_row - 6, 0);
    
    TERM_CORS_CLEAR;

    for (i = 0; i < size.ws_col; ++i)
    {
        printf("\033[31m-\033[0m");
    }
    printf("\n");

    TERM_CORS_CLEAR;

    printf("\033[36mPACKAGE NUM \t: %d\033[0m", ++pkt_num);
    
    TERM_CORS_MOVE(size.ws_row - 5, size.ws_col - 42);

    totol_length += length;

    time(&timep);

    if (time_start == timep)
    {
        printf("\033[36mSPEED        : %d Bytes/S\033[0m\n", totol_length);
    }
    else
    {
        printf("\033[36mSPEED        : %d Bytes/S\033[0m\n", totol_length / (timep - time_start));
    }

    TERM_CORS_CLEAR;

    for (i = 0; i < size.ws_col; ++i)
    {
        printf("\033[31m-\033[0m");
    }
    printf("\n");

    TERM_CORS_CLEAR;

    printf("\033[36mLENGTH \t\t: %d\033[0m", length);

    TERM_CORS_MOVE(size.ws_row - 3, size.ws_col - 42);

    printf("\033[36mSTART TIME   : %s\033[0m", ctime(&time_start));

    TERM_CORS_CLEAR;

    for (i = 0; i < size.ws_col; ++i)
    {
        printf("\033[31m-\033[0m");
    }
    printf("\n");

    TERM_CORS_CLEAR;

    printf("\033[36mTOTAL LENGTH \t: %d\033[0m", totol_length);

    TERM_CORS_MOVE(size.ws_row - 1, size.ws_col - 42);
    
    printf("\033[36mCURRENT TIME : %s\033[0m", ctime(&timep));

    for (i = 0; i < size.ws_col; ++i)
    {
        printf("\033[31m-\033[0m");
    }
    printf("\n");

    return 0;
}

int main()
{
    char *string = (char *)malloc(10240);
    int length = 0;
    int i = 0;
    int times = 0;

    srand((unsigned int)time(NULL));

    times = rand() % 256;

    time(&time_start);

    for (i = 0; i < times; ++i)
    {
        auto_string_create(string, 10240 - 1, &length);

        auto_string_show(string, length);

        usleep(200 * 1000);
    }

    free(string);

    return 0;
}
