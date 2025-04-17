/**
 * @file debug.h
 * @author -
 * @brief Dedicated header file for debug only functions (typically logging
 * functions).
 * @version 1.0.1
 * @date 07/03/2025
 */
#ifndef _DEBUG_H
#define _DEBUG_H

#include "header.h"

/**
 * @brief Prints out all contents of the header_data instance.
 *
 * @param data The header_data instance to be printed. Can be NULL.
 */
void print_data (header_data *data);

#endif