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
#include <netdb.h>
#include "utils.h"
// https://nmon.sourceforge.io/pmwiki.php?n=Site.Nweb
#define MAX_SIZE 4096
#define TCP 0
#define LOCALHOST "127.0.0.1"
uint16_t port;
typedef int file_descriptor;
int main(int argc, char const *argv[])
{
    pid_t id = getpid();
    handle_args(argc, argv);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    // listener socket and connection socket
    // getting the fd from the socket() syscall using IPv4 and TCP
    file_descriptor server = socket(AF_INET, SOCK_STREAM, TCP), connection;
    if (server == -1)
    {
        exit_error("Server socket error");
    }
    // set server struct
    serv_addr.sin_addr.s_addr = inet_addr(LOCALHOST);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (bind(server, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        exit_error("Error with bind()");
    }
    if (listen(server, 10) == -1)
    {
        exit_error("Error with listen()");
    }
    while (1)
    {
        // TODO: fork and handle connections
    }
    if (id != 0)
        close(server);
    return 0;
}
