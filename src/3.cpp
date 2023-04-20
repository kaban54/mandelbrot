#include "mandelbrot.h"

void get_img (sf::Image &img)
{
    float x2 [8] = {};
    float y2 [8] = {};
    float xy [8] = {};
    float coefs [8] = {0, 1, 2, 3, 4, 5, 6, 7};

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
                float x0    [8] = {};
                float xnext [8] = {};
                float y0    [8] = {};
                float ynext [8] = {};
                float r2    [8] = {};
                mm_set_i1 (count, -1);

                mm_set_ps1 (x0, dx);
                mm_mul_ps  (x0, coefs);
                mm_add_ps1 (x0, x);
                mm_cpy_ps  (xnext, x0);

                mm_set_ps1 (y0, y);
                mm_set_ps1 (ynext, y);

                int out = 0;

                for (int iter = 0; iter < MAXITER; iter++)
                {
                    mm_cpy_ps (x2, xnext);
                    mm_mul_ps (x2, x2);

                    mm_cpy_ps (y2, ynext);
                    mm_mul_ps (y2, y2);
                        
                    mm_cpy_ps (xy, xnext);
                    mm_mul_ps (xy, ynext);

                    mm_cpy_ps (r2, x2);
                    mm_add_ps (r2, x2);

                    for (i = 0; i < 8; i++)
                    {
                        if (!(out & (1 << i)) && r2 [i] > MAXR2)
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

                    mm_cpy_ps (xnext, x0);
                    mm_add_ps (xnext, x2);
                    mm_sub_ps (xnext, y2);
                        
                    mm_cpy_ps (ynext, y0);
                    mm_add_ps (ynext, xy);
                    mm_add_ps (ynext, xy);
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