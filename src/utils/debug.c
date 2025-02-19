#include <stdio.h>
#include "debug.h"

void print_data(header_data *data)
{
    header current = NULL;
    if (!data)
    {
        printf("NULL\n");
    }
    else
    {
        printf("header_data: {\n"
               "\tmethod: \"%s\"\n"
               "\tpath: \"%s\"\n"
               "\tversion: \"%s\"\n"
               "\theaders: {\n",
               data->method ? data->method : "NULL",
               data->path ? data->path : "NULL",
               data->version ? data->version : "NULL");
        current = data->header_list.head;
        while (current)
        {
            printf("\t\t\"%s\": ", current->key ? current->key : "NULL");
            printf("\"%s\"\n", current->value ? current->value : "NULL");
            current = current->next;
        }
        printf("\t}\n}\n");
    }
}