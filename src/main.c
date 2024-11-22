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
#define DEFAULT_PATH "serverconfig.json"
volatile sig_atomic_t running = 1;
file_descriptor server = 0, connection = 0;

int main(int argc, char const *argv[])
{
    pid_t id = getpid();
    const char *file_path = handle_args(argc, argv);
    //  listener socket and connection socket
    //  getting the fd from the socket() syscall using IPv4 and TCP

    server = socket(AF_INET, SOCK_STREAM, TCP);
    if (server == -1)
    {
        exit_error("Server socket error");
    }
    server_config *config = load_config(file_path ? file_path : DEFAULT_PATH);
    // set server struct
    sockaddr_in_p addr = make_ipv4(config);
    // initialize client struct and its length
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    memset(&client_addr, 0, sizeof(client_addr));
    // bind server to socket and listen
    if (bind(server, (struct sockaddr *)addr, sizeof(*addr)) == -1)
    {
        free(config);
        exit_error("cannot bind to port %u", ntohs(addr->sin_port));
    }
    if (listen(server, config->max_clients) == -1)
    {
        free(config);
        exit_error("listen() failed");
    }
    buffers *bufs = make_buffers(config);
    struct timeval interval = {config->timeout_s, 0};
    // Start interrupt handler
    free(config);
    config = NULL;
    signal(SIGINT, signal_handler);
    while (running)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server, &read_fds);
        // 1 for 1 second interval, 0 for 0 (additional) nanoseconds;
        int ret = select(server + 1, &read_fds, NULL, NULL, &interval);
        if (ret == -1)
        {
            free_bufs(bufs);
            exit_error("select failed");
        }
        if (ret == 0 || !FD_ISSET(server, &read_fds))
            continue;
        connection = accept(server, (struct sockaddr *)&client_addr, &addr_len);
        if (connection == -1)
        {
            warning("connection failed.");
            continue;
        }
        id = fork();
        if (id == -1)
        {
            warning("could not fork. Closing connection.");
            close(connection);
            continue;
        }
        if (id == 0)
        {
            close(server);
            // TODO: Handle connectino
            close(connection);
            goto end;
        }
        else
        {
            close(connection);
        }
    }
    if (id != 0)
        close(server);
end:
    free_bufs(bufs);
    return 0;
}
