#include <stdio.h>

#include "cli_server.h"

int cli_server_init()
{
    return 0;
}

int cli_server_work_step()
{
    return 0;
}

int cli_server_work_loop()
{
    while (1) {
        cli_server_work_step();
    }

    return 0;
}

int cli_server_exit()
{
    return 0;
}