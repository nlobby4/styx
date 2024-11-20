#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include "utils.h"
#include "types.h"
#include "mem.h"
// https://nmon.sourceforge.io/pmwiki.php?n=Site.Nweb
#define BUF_SIZE 4096
#define TCP 0
#define LOCALHOST "127.0.0.1"
uint16_t port;
volatile sig_atomic_t running = 1;

int main(int argc, char const *argv[])
{
    pid_t id = getpid();
    handle_args(argc, argv);
    // listener socket and connection socket
    // getting the fd from the socket() syscall using IPv4 and TCP
    file_descriptor server = socket(AF_INET, SOCK_STREAM, TCP);
    if (server == -1)
    {
        close(server);
        exit_error("Server socket error");
    }
    // set server struct
    sockaddr_in_p addr = make_ipv4(LOCALHOST, port);
    socklen_t addr_len = sizeof(*addr);
    if (bind(server, (struct sockaddr *)addr, sizeof(*addr)) == -1)
    {
        close(server);
        exit_error("Error with bind()");
    }
    if (listen(server, 10) == -1)
    {
        close(server);
        exit_error("Error with listen()");
    }
    while (running)
    {
        // TODO: Handle blocking by accept() and handle connections
        signal(SIGINT, signal_handler);
        file_descriptor connection = accept(server, (struct sockaddr *)addr, &addr_len);
        if (connection == -1)
        {
            perror("Error with accept(). Check connection.\n");
            continue;
        }
        id = fork();
        if (id == -1)
        {
            perror("Error with fork(). Closing connection.\n");
            continue;
        }
        if (id == 0)
        {
            close(server);
            // Do stuff
            close(connection);
            return 0;
        }
        else
            close(connection);
    }
    if (id != 0)
        close(server);
    return 0;
}
