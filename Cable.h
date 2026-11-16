#ifndef CABLE_H
#define CABLE_H

// todo: fprint_table() that allows to print to a given stream
// todo: allow data to be a string
// todo: macro for debugging that allows to show/hide error and/or warn messages
//  (maybe one macro for errors and one macro for warnings)

#define MAX_COLUMN_WIDTH 128

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// on error returns value < 0
static ssize_t get_max_col_width(size_t n, char data_type, const void *data)
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
                    fprintf(stderr, "Cable WARN: Data in row exceeded max column width = %zu\n", i + 1);
                    str_length = MAX_COLUMN_WIDTH - 1;
                }
                max_width = max_width >= (size_t)str_length ? max_width : str_length;
                printf("buffer='%s', length = %d\n", buffer, str_length);
                break;
            default:
                fprintf(stderr, "Cable ERR: get_max_col_width() unsupported data type '%c'\n", data_type);
                return -1;
        }
    }
    printf("found max width: %zd\n\n", (ssize_t)max_width);
    return (ssize_t)max_width;
}

// on error returns value < 0
static ssize_t get_data_type_size(char data_type)
{
    switch (data_type)
    {
        case 'd':
            return (ssize_t)sizeof(int);
            break;
        default:
            fprintf(stderr, "Cable ERR: get_data_type_size unsupported data type '%c'\n", data_type);
            return -1;
    }
}

// todo: add precision and proper width for floats
// todo: add left / right alignment.
static int print_table_cell(int width, char data_type, const void *value)
{
    // todo set precision if value should to be truncated
    switch (data_type)
    {
        case 'd':
            printf("%*d", width, *(int *)value);
            // printf("%-.*d", width, *(int *)value);
            break;
        default:
            fprintf(stderr, "Cable ERR: print_table_cell() unsupported data type '%c'\n", data_type);
            return -1;
    }
    return 0;
}

/// c - number of columns.
/// r - number of rows.
/// headers - array of strings that will be printed on the top of the table.
/// data_format - string that tells the function what datatypes have arrays stored
/// inside of the 'data'.
/// data_format can be build as a combination of the following chars:
/// d - int,
/// f - float, //todo
/// e.g: "dff" means that data points to 3 arrays of type (int[], float[], float[]).
int print_table( //
    size_t c,
    size_t r,
    char *const headers[c],
    const char data_format[c],
    void *const *data)
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
            fprintf(stderr, "Cable ERR: get_max_col_width returned with %zd\n", data_max_width);
            free(column_widths);
            return EXIT_FAILURE;
        }
        max_width = max_width >= (size_t)data_max_width ? max_width : data_max_width;
        column_widths[i] = max_width;
    }
    // todo: print headers

    for (size_t j = 0; j < r; j++)
    {
        for (size_t i = 0; i < c; i++)
        {
            const void *current_column = data[i];
            ptrdiff_t row_offset = (ptrdiff_t)j * get_data_type_size(data_format[i]);
            // printf("j: %zu, i: %zu, row_offset: %td\n", j, i, row_offset);
            const void *current_value = &((unsigned char *)current_column)[row_offset];
            putchar('|');
            print_table_cell((int)column_widths[i], data_format[i], current_value);
            putchar('|');
        }
        putchar('\n');
    }

    free(column_widths);
    return 0;
}

#endif // CABLE_Hsize_t c,
