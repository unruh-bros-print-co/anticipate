#ifndef BITMAP_INFO_H
#define BITMAP_INFO_H

#include <stdint.h>
#include <pebble.h>

typedef struct {
    GBitmap* gbitmap;
    uint16_t width;
    uint16_t height;
} BitmapInfo;

#endif