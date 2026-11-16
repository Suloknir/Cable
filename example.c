#include "Cable.h"
#include <stddef.h>
#include <stdio.h>

int handle_error(void)
{
    fprintf(stderr, "ERR: Cable function returned with error\n");
    exit(EXIT_FAILURE);
}

int main(void)
{
    char * const headers[] = {"sizes", "weight", "some very long header"};
    const size_t columns = sizeof(headers) / sizeof(*headers);

    const int sizes[] = {10, 100, 1000, 10000};
    const int weights[] = {12, 0, 21938243, -75};
    const int random[] = {-1, 23, 14, -0};
    const size_t rows = sizeof(sizes) / sizeof(*sizes);

    void *data[columns];
    data[0] = (void *)sizes;
    data[1] = (void *)weights;
    data[2] = (void *)random;
    print_table(columns, rows, headers, "ddd", data) && handle_error();
    // printf("int: |%.*d|\n", 2, 2193);
    // printf("float: |%3.*f|\n", 3, 2193.231732f);
    // printf("string: |%*s|\n", 7, "das");
    // printf("string truncated: |%.*s|\n", 2, "das");
    return 0;
}
