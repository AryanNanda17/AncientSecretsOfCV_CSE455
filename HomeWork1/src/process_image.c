#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"
// The image has three channels... they are stored in image.data in a linear format. Firstly, the channel one pixels, then channel 2
// pixels and then channel three pixels. So in order to get a specific pixel from image.data, do this (c * imW * imH + y * imW + x) where
// x is column no. and y is row no. of the pixel you want.
float get_pixel(image im, int x, int y, int c)
{
    int imW = im.w;
    int imH = im.h;
    int imC = im.c;
    // Clamping(bound)
    if (x < 0)
        x = 0;
    else if (x >= imW)
        x = imW - 1;

    if (y < 0)
        y = 0;
    else if (y >= imH)
        y = imH - 1;

    if (c < 0)
        c = 0;
    else if (c >= imC)
        c = imC - 1;
    // finding index
    int idx = c * imW * imH + y * imW + x;
    return im.data[idx];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    int imW = im.w;
    int imH = im.h;
    int imC = im.c;
    // Checking if the pixel no. is in bound if not then simply return.
    if (x < 0 || x >= imW || y < 0 || y >= imH || c < 0 || c >= imC)
        return;
    int idx = c * imW * imH + y * imW + x;
    im.data[idx] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    memcpy(copy.data, im.data, sizeof(float) * im.w * im.h * im.c);
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for (int y = 0; y < im.h; ++y)
    {
        for (int x = 0; x < im.w; ++x)
        {
            gray.data[y * im.w + x] = 0.299 * im.data[0 * im.w * im.h + y * im.w + x] + 0.587 * im.data[1 * im.w * im.h + y * im.w + x] +
                                      0.114 * im.data[2 * im.w * im.h + y * im.w + x];
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    if (c < 0 || c >= im.c)
        return;

    for (int y = 0; y < im.h; ++y)
    {
        for (int x = 0; x < im.w; ++x)
        {
            im.data[c * im.w * im.h + y * im.w + x] += v;
        }
    }
}

void clamp_image(image im)
{
    for (int c = 0; c < im.c; ++c)
    {
        for (int y = 0; y < im.h; ++y)
        {
            for (int x = 0; x < im.w; ++x)
            {
                int idx = c * im.w * im.h + y * im.w + x;
                im.data[idx] = fmax(im.data[idx], 0.0);
                im.data[idx] = fmin(im.data[idx], 1.0);
            }
        }
    }
}

// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

void rgb_to_hsv(image im)
{
    float r, g, b, min, max, c, h, s, v;
    for (int y = 0; y < im.h; y++)
    {
        for (int x = 0; x < im.w; x++)
        {
            r = im.data[x + y * im.w];
            g = im.data[x + y * im.w + im.w * im.h];
            b = im.data[x + y * im.w + 2 * im.w * im.h];
            min = three_way_min(r, g, b);
            max = three_way_max(r, g, b);
            c = max - min;
            v = max;
            if (v != 0)
            {
                s = c / v;
            }
            else
            {
                s = 0;
            }

            if (c != 0)
            {
                if (v == r)
                {
                    h = (g - b) / c;
                }
                else if (v == g)
                {
                    h = ((b - r) / c) + 2;
                }
                else
                {
                    h = ((r - g) / c) + 4;
                }
            }
            else
            {
                h = 0;
            }
            h /= 6;
            if (h < 0)
            {
                h++;
            }
            im.data[x + y * im.w] = h;
            im.data[x + y * im.w + im.w * im.h] = s;
            im.data[x + y * im.w + 2 * im.w * im.h] = v;
        }
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    float h, s, v, c, max, min, r, g, b, h_temp;
    for (int y = 0; y < im.h; y++)
    {
        for (int x = 0; x < im.w; x++)
        {
            h = im.data[x + y * im.w];
            s = im.data[x + y * im.w + im.w * im.h];
            v = im.data[x + y * im.w + 2 * im.w * im.h];
            c = s * v;
            max = v;
            min = v - c;
            h_temp = h * 6;
        }
    }
