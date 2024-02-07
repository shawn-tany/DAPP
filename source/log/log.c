#include "log.h"

int log_init(void)
{
    openlog(const char *ident, int option, int facility);

    return 0;
}

int log_trace(LOG_LEVEL_T level, const char *title, const char *log_content)
{
    syslog(int priority, const char *format, ...);

    return 0;
}

int log_uinit(void)
{
    closelog();

    return 0;
}