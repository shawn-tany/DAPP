#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

enum 
{
    MONITOR_FAILED = 1,
    MONITOR_RUNNING,
    MONITOR_SLEEP,
} MONITOR_STATUS;

enum
{
    DEAMON_FAILED = 1,
    DEAMON_RUNNING,
    DEAMON_RESTART,
} DEAMON_STATUS;

static int g_deamon_status = 0;

static void signal_handle(int sig)
{
   if (SIGINT == sig)
   {
       g_deamon_status = DEAMON_FAILED;
   }
}

static int program_monitor(char *program)
{
    int program_status = MONITOR_SLEEP;
    FILE *pfp = NULL;
    char cmd[256] = { 0 };
    char line[1024] = { 0 };

    snprintf(cmd, sizeof(cmd), "pidof %s", program);

    if (!(pfp = popen(cmd, "r")))
    {
        perror("popen error");
        return MONITOR_FAILED;
    }

    while (fgets(line, sizeof(line), pfp))
    {
        char *pstart = line;
        char *pend = NULL;

        while (pstart && ((pend = strchr(pstart, ' ')) || (pend = strchr(pstart, '\n'))))
        {
            *pend = 0;

            char proc_pid[64] = { 0 };
            
            snprintf(proc_pid, sizeof(proc_pid), "/proc/%s", pstart);

            if (!access(proc_pid, 0))
            {
                program_status = MONITOR_RUNNING;
            }

            pstart = pend + 1;
        }
    }

    pclose(pfp);

    return program_status;
}

static int deamon(int argc, char *argv[])
{
    char *program = argv[0];
    int ret = program_monitor(program);
    
    if (MONITOR_FAILED == ret)
    {
        printf("program(%s) monitor failed\n", program);
        return DEAMON_FAILED;
    }
    else if (MONITOR_SLEEP == ret)
    {
        signal(SIGINT, signal_handle);

        pid_t pid = 0;

        pid = fork();

        if (-1 == pid)
        {
            perror("deamon fork");
            return DEAMON_FAILED;
        }
        else if (0 < pid)
        {
            sleep(5);

            if (DEAMON_FAILED == g_deamon_status)
            {
                return DEAMON_FAILED;
            }
            else
            {
                return DEAMON_RESTART;
            }
        }
        else
        {
            printf("deamon start(%s)\n", program);
            if (0 > execvp(program, argv))
            {
                printf("deamon finish(%s)\n", program);
                perror("execvp error");
            
                kill(getppid(), 2);
                exit(1);
            }
        }
    }

    return DEAMON_RUNNING;
}

int main(int argc, char *argv[])
{
    pid_t pid = 0;
    int restart_times = 0;
    char *program = argv[1];

    if (2 > argc)
    {
        printf("please select a program\n");
        return -1;
    }

    pid = fork();

    if (-1 > pid)
    {
        perror("fork error");
        return -1;
    }
    else if (0 < pid)
    {
        ;
    }
    else
    {
        pid = setsid();

        if (-1 == pid)
        {
            perror("setsid error");
            return -1;
        }

        while (1)
        {
            int ret = deamon(argc - 1, &argv[1]);

            if (DEAMON_FAILED == ret)
            {
                printf("deamon %s failed\n", program);
                return -1;
            }
            else if (DEAMON_RESTART == ret)
            {
                printf("deamon %d times\n", ++restart_times);
            }
        }
    }

    return 0;
}
