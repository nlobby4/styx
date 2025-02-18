#pragma once

typedef struct _header
{
    char *key;
    char *value;
    struct _header *next;
} *header;

typedef struct
{
    char *method;
    char *path;
    char *version;
    struct list
    {
        header head;
    } header_list;
} header_data;

header_data *parse(char *header_str);