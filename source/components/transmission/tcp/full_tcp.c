#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "full_tcp.h"

int full_tcp_server_socket()
{
    int sock = 0;

    sock = socket();

    if (-1 == sock)
    {
        perror("socket");
        return -1;
    }

    return sock;
}

int full_tcp_client_socket()
{
    int sock = 0;

    sock = socket();

    if (-1 == sock)
    {
        perror("socket");
        return -1;
    }

    return sock;
}

int full_tcp_receive(int sock, int size)
{
    return 0;
}

int full_tcp_receive_wait()
{
    return 0;
}

int full_tcp_receive_epoll()
{
    return 0;
}

int full_tcp_sendto()
{
    return 0;
}

int full_tcp_sendto_wait()
{
    return 0;
}

int full_tcp_close(int sock)
{
    close(sock);

    return 0;
}