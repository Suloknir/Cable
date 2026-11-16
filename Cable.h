#ifndef CABLE_H
#define CABLE_H

// todo: fprint_table() that allows to print to a given stream
// todo: add printf-like data_format format
// todo: change print_table to variadic function and remove 'void **data' parameter
// todo: add string data_type
// todo: create macro for debugging that allows to show/hide error and/or warn messages
//  (maybe one macro for errors and one macro for warnings)

#define MAX_COLUMN_WIDTH 12

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// on error returns value < 0
static ssize_t get_max_col_width(size_t n, char data_type, const void *data)
{
    static char buffer[MAX_COLUMN_WIDTH + 1];
    size_t max_width = 0;
    for (size_t i = 0; i < n; i++)
    {
        int str_length;
        switch (data_type)
        {
            case 'd':
                str_length = snprintf(buffer, MAX_COLUMN_WIDTH + 1, "%d", ((int *)data)[i]);
                if (str_length >= MAX_COLUMN_WIDTH)
                {
                    fprintf(stderr,
                        "Cable WARN: Data in row %zu exceeded max column width = %d\n",
                        i + 1,
                        MAX_COLUMN_WIDTH);
                    // str_length = MAX_COLUMN_WIDTH - 1;
                }
                max_width = max_width >= (size_t)str_length ? max_width : str_length;
                // printf("buffer='%s', length = %d\n", buffer, str_length);
                break;
            default:
                fprintf(stderr, "Cable ERR: get_max_col_width() unsupported data type '%c'\n", data_type);
                return -1;
        }
    }
    // printf("found max width: %zd\n\n", (ssize_t)max_width);
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
    static char cell_string_buffer[MAX_COLUMN_WIDTH + 1];
    snprintf(cell_string_buffer, MAX_COLUMN_WIDTH + 1, "");

    int length = 0;
    switch (data_type)
    {
        case 'd':
            length = snprintf(cell_string_buffer, MAX_COLUMN_WIDTH + 1, "%d", *(int *)value);
            break;
        case 's':
            length = snprintf(cell_string_buffer, MAX_COLUMN_WIDTH + 1, "%s", (const char *)value);
            break;
        default:
            fprintf(stderr, "Cable ERR: print_table_cell() unsupported data type '%c'\n", data_type);
            return -1;
    }
    if (length >= MAX_COLUMN_WIDTH + 1)
        return printf("%.*s", MAX_COLUMN_WIDTH, cell_string_buffer);
    if (width > MAX_COLUMN_WIDTH)
        return printf("%*s", MAX_COLUMN_WIDTH, cell_string_buffer);
    return printf("%*s", width, cell_string_buffer);
}

/// c - number of columns.
/// r - number of rows.
/// headers - array of strings that will be printed on the top of the table.
/// data_format - string that tells the function what datatypes are stored
/// inside of the 'data'.
/// data_format can be build as a combination of the following characters:
/// d - int,
/// f - float, //todo
/// e.g: "dff" means that data points to 3 arrays of type (int[], float[], float[]).
/// returns 0 on succes
int print_table( //
    size_t c,
    size_t r,
    char *const headers[c],
    const char data_format[c],
    void *const *data)
{
    if (strlen(data_format) != c)
    {
        fprintf(stderr, "Cable ERR: data_format length != number of columns\n");
        return EXIT_FAILURE;
    }
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
            fprintf(stderr, "Cable ERR: get_max_col_width() returned with %zd\n", data_max_width);
            free(column_widths);
            return EXIT_FAILURE;
        }
        max_width = max_width >= (size_t)data_max_width ? max_width : data_max_width;
        column_widths[i] = max_width;
    }
    // for (size_t i = 0; i < c; i++)
    // printf("|%*s", (int)column_widths[i], headers[i]);
    for (size_t i = 0; i < c; i++)
    {
        putchar('|');
        print_table_cell((int)column_widths[i], 's', headers[i]);
    }
    printf("|\n");

    for (size_t row = 0; row < r; row++)
    {
        for (size_t col = 0; col < c; col++)
        {
            const void *current_column = data[col];
            const ssize_t data_type_size = get_data_type_size(data_format[col]);
            if (data_type_size < 0)
            {
                fprintf(stderr, "Cable ERR: get_data_type_size() returned with %zd\n", data_type_size);
                free(column_widths);
                return EXIT_FAILURE;
            }
            ptrdiff_t row_offset = (ptrdiff_t)row * data_type_size;
            const void *current_value = &((unsigned char *)current_column)[row_offset];
            putchar('|');
            print_table_cell((int)column_widths[col], data_format[col], current_value);
        }
        printf("|\n");
    }
    free(column_widths);
    return 0;
}

#endif // CABLE_H
