#pragma once
#include "types.h"
#include "mem.h"

void handle_connection(file_descriptor server, file_descriptor connection, message_buffers *bufs);