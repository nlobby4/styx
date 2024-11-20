#pragma once
#include "types.h"

/**
 * @brief Creates and returns a pointer to a ipv4 socket address struct.
 * @param ip IPv4 Address in dotted notation.
 * @param port The port to be used.
 * @return A pointer to a singleton socket address struct.
 * Initializes a singleton, zeroes it and sets all values, then returns a pointer to it.
 */
sockaddr_in_p make_ipv4(const char *ip, uint16_t port);