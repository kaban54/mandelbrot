#include "mandelbrot.h"

void get_img (sf::Image &img)
{
    float x2 [8] = {};
    float y2 [8] = {};
    float xy [8] = {};

    float x    = 0, y    = 0;
    int   xpix = 0, ypix = 0;

    int i = 0;

    for (ypix = 0, y = minY; ypix < H ; ypix++, y += dy)
    {
        for (xpix = 0, x = minX; xpix < W; xpix += 8, x += dx * 8)
        {
            int count [8] = {-1, -1, -1, -1, -1, -1, -1, -1};

            for (int rep = 0; rep < REP_NUM; rep ++)
            {
                float x0    [8] = {x, x + dx, x + dx * 2, x + dx * 3, x + dx * 4, x + dx * 5, x + dx * 6, x + dx * 7};
                float xnext [8] = {x, x + dx, x + dx * 2, x + dx * 3, x + dx * 4, x + dx * 5, x + dx * 6, x + dx * 7};
                float y0    [8] = {y, y, y, y, y, y, y, y};
                float ynext [8] = {y, y, y, y, y, y, y, y};

                for (i = 0; i < 8; i++) count [i] = -1;

                int out = 0;

                for (int iter = 0; iter < MAXITER; iter++)
                {
                    for (i = 0; i < 8; i++) x2 [i] = xnext [i] * xnext [i];
                    for (i = 0; i < 8; i++) y2 [i] = ynext [i] * ynext [i];
                    for (i = 0; i < 8; i++) xy [i] = xnext [i] * ynext [i];
                    for (i = 0; i < 8; i++)
                    {
                        if (!(out & (1 << i)) && x2 [i] + y2 [i] > MAXR2)
                        {
                            count [i] = iter;
                            x2 [i] = 0;
                            y2 [i] = 0;
                            xy [i] = 0;
                            x0 [i] = 0;
                            y0 [i] = 0;
                            out |= 1 << i;
                        }
                    }

                    if (out == 0b11111111) break;

                    for (i = 0; i < 8; i++) xnext [i] = x2 [i] - y2 [i] + x0 [i];
                    for (i = 0; i < 8; i++) ynext [i] = 2 * xy [i]      + y0 [i];
                }
            }
            #ifdef _DRAW
            for (i = 0; i < 8; i++)
            {
                if (count [i] == -1) img.setPixel (xpix + i, ypix, sf::Color::Black);
                else                 img.setPixel (xpix + i, ypix, sf::Color (count [i] % 256, 0, count [i] % 256 / 2));
            }
            #endif
        }
    }
}