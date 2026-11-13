#ifndef CABLE_H
#define CABLE_H

// todo: fprint_table() that allows to print to a given stream
// todo: macro for debugging that allows to show/hide error and/or warn messages
//  (maybe one macro for errors and one macro for warnings)

#define MAX_COLUMN_WIDTH 64

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// returns value < 0 on error
static ssize_t get_max_col_width(size_t n, char data_type, void *data)
{
    char buffer[MAX_COLUMN_WIDTH];
    size_t max_width = 0;
    for (size_t i = 0; i < n; i++)
    {
        int str_length;
        // todo: other data_types
        switch (data_type)
        {
            case 'd':
                str_length = snprintf(buffer, MAX_COLUMN_WIDTH, "%d", ((int *)data)[i]);
                if (str_length >= MAX_COLUMN_WIDTH)
                {
                    fprintf(stderr, "Cable WARN: Data in row exceeded max column width = %zu", i + 1);
                    str_length = MAX_COLUMN_WIDTH - 1;
                }
                max_width = max_width >= (size_t)str_length ? max_width : str_length;
                printf("buffer='%s', length = %d\n", buffer, str_length);
                break;
            default:
                fprintf(stderr, "Cable ERR: unsopported data format '%c'\n", data_type);
                return -1;
        }
    }
    printf("found max width: %zd\n\n", (ssize_t)max_width);
    return (ssize_t)max_width;
}

/// c - number of columns.
/// r - number of rows.
/// headers - array of strings that will be printed on the top of the table.
/// data_format - string that tells the function what datatypes have arrays stored
/// inside of the 'data'.
/// data_format can be build as a combination of the following chars:
/// todo s - string,
/// d - int,
/// todo f - float,
/// e.g: "dsf" means that data points to 3 arrays of type (int[], char*[], float[]).
int print_table( //
    size_t c,
    size_t r,
    const char *headers[static c],
    const char data_format[static c],
    void **data)
{
    size_t *column_widths = malloc(c * sizeof(size_t));
    if (column_widths == NULL)
    {
        fprintf(stderr, "Cable ERR: malloc\n");
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < c; i++)
    {
        size_t max_width = strlen(headers[i]);
        ssize_t data_max_width = get_max_col_width(r, data_format[i], data[i]);
        if (data_max_width < 0)
        {
            fprintf(stderr, "Cable ERR: get_max_col_width returned with %zu\n", data_max_width);
            free(column_widths);
            return EXIT_FAILURE;
        }
        max_width = max_width >= (size_t)data_max_width ? max_width : data_max_width;
        column_widths[i] = max_width;
    }
    free(column_widths);
    return 0;
}

#endif // CABLE_H
