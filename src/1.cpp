#include "mandelbrot.h"

void get_img (sf::Image &img)
{
    float x2 = 0;
    float y2 = 0;
    float xy = 0;

    float x0   = 0, y0   = 0;
    int   xpix = 0, ypix = 0;

    for (ypix = 0, y0 = minY; ypix < H ; ypix++, y0 += dy)
    {
        for (xpix = 0, x0 = minX; xpix < W; xpix++, x0 += dx)
        {
            int count = 0;
            for (int rep = 0; rep < REP_NUM; rep ++)
            {
                float x = x0;
                float y = y0;
                count = 0;

                for (count = 0; count < MAXITER; count++)
                {
                    x2 = x * x;
                    y2 = y * y;
                    xy = x * y;

                    if (x2 + y2 > MAXR2) break;

                    x = x2 - y2 + x0;
                    y = 2 * xy  + y0;
                }
            }

            #ifdef _DRAW
            if (count == MAXITER) img.setPixel (xpix, ypix, sf::Color (0, 0, 0));
            else                  img.setPixel (xpix, ypix, sf::Color (count % 256, 0, count % 256 / 2));
            #endif
        }
    }
}
