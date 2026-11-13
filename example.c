#include "Cable.h"
#include <stdio.h>

int main(void)
{
    const char *headers[] = {"sizes", "weight"};
    const size_t columns = sizeof(headers) / sizeof(*headers);

    const int sizes[] = {10, 100, 1000, 10000};
    const int weights[] = {12, 0, 21938243, -75};
    const size_t rows = sizeof(sizes) / sizeof(*sizes);

    void *data[columns];
    data[0] = (void *)sizes;
    data[1] = (void *)weights;
    int ret = print_table(columns, rows, headers, "dd", data);
    if (ret != 0)
    {
        fprintf(stderr, "ERR: print_table returned with %d\n", ret);
        return 1;
    }
    return 0;
}
