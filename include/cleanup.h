/**
 * @file cleanup.h
 * @author Alexsander d. S. Tenne
 * @brief Small header file containing the cleanup function, which is to be
 * called before exiting the program, in order to free up any ressources.
 * @version 1.0.0
 * @date 2025-03-05
 */
#ifndef _CLEANUP_H
#define _CLEANUP_H

#include "buf.h"
void cleanup (message_buffers *bufs);

#endif