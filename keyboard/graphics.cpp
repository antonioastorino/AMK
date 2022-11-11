#include "graphics.h"
static uint8_t bitmap[DISPLAY_COLS][DISPLAY_PAGES];

void graphics_add_pixel(uint8_t x, uint8_t y)
{
    uint8_t page = y / 8;
    uint8_t b    = 1 << (y % 8);
    bitmap[x][page] |= b;
}

void graphics_clear_buffer()
{
    // TODO: use memset()
    for (uint8_t p = 0; p < DISPLAY_PAGES; p++)
    {
        for (uint8_t c = 0; c < DISPLAY_COLS; c++)
        {
            bitmap[c][p] = 0x00;
        }
    }
}

uint8_t (*graphics_get_bitmap())[DISPLAY_COLS][DISPLAY_PAGES] { return &bitmap; }
