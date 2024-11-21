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
#include <sys/select.h>
#include <sys/time.h>
#include "utils.h"
#include "types.h"
#include "mem.h"
#include "config.h"
// https://nmon.sourceforge.io/pmwiki.php?n=Site.Nweb
#define TCP 0
#define MAX_CLIENTS 10
uint16_t port;
volatile sig_atomic_t running = 1;
// TODO: Handle errno properly

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
    sockaddr_in_p addr = make_ipv4("127.0.0.1", port);
    // initialize client struct and its length
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    memset(&client_addr, 0, sizeof(client_addr));
    // bind server to socket and listen
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
    // Start interrupt handler
    signal(SIGINT, signal_handler);
    while (running)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server, &read_fds);
        // 1 for 1 second interval, 0 for 0 (additional) nanoseconds;
        struct timeval interval = {1, 0};
        int ret = select(server + 1, &read_fds, NULL, NULL, &interval);
        if (ret <= 0 || !FD_ISSET(server, &read_fds))
        {
            puts("Timeout");
            continue;
        }
        file_descriptor connection = accept(server, (struct sockaddr *)&client_addr, &addr_len);
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

            close(connection);
            return 0;
        }
        else
        {
            close(connection);
        }
    }
    if (id != 0)
        close(server);
    return 0;
}
