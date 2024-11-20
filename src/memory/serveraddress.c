#include "types.h"
#include "mem.h"
#include <inttypes.h>
#include <string.h>
#include <arpa/inet.h>
sockaddr_in_p make_ipv4(const char *ip, uint16_t port)
{
    static struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (ip)
        inet_pton(AF_INET, ip, &addr.sin_addr.s_addr);
    else
        addr.sin_addr.s_addr = INADDR_ANY;
    return &addr;
}