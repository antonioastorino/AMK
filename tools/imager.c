#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define MAX_IN_COLS (4)
#define MAX_IN_ROWS (8)
#define MAX_IN_SIZE                                                                                \
    (MAX_IN_ROWS * (MAX_IN_COLS + 1)) /* The newline character requires an extra col */
#define MAX_OUT_COLS MAX_IN_COLS
#define MAX_OUT_PAGES (MAX_IN_ROWS / 8)

int main(int argc, char* argv[])
{
    size_t page                                    = 0;
    size_t col                                     = 0;
    size_t row                                     = 0;
    uint8_t out_image[MAX_OUT_COLS][MAX_OUT_PAGES] = {0};
    char buff[MAX_IN_SIZE]                         = {0};
    size_t counter                                 = 0;

    if (argc < 2)
    {
        printf("File not specified");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return 2;
    }
    ssize_t bytes_read = read(fd, buff, MAX_IN_SIZE);
    if (bytes_read < 0)
    {
        perror("read");
        return 3;
    }
    printf("Read: %zd bytes\n", bytes_read);

    for (size_t b = 0; b < bytes_read; b++)
    {
        if (buff[b] == '\n')
        {
            // End of line reached
            continue;
        }
        row  = counter / MAX_IN_COLS;
        col  = counter % MAX_IN_COLS;
        page = row / 8;
        out_image[col][page] |= (buff[b] == '0') << row;
        printf("Page: %zu Col %zu Val %d\n", page, col, buff[b] == '0');
        counter++;
    }

    printf("{\n");
    for (size_t p = 0; p < MAX_OUT_PAGES; p++)
    {
        printf("{");
        for (size_t c = 0; c < MAX_OUT_COLS; c++)
        {
            printf("0x%02x, ", out_image[c][p]);
        }
        printf("}\n");
    }
    printf("}\n");
    return 0;
}
