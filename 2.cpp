#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


void DrawMandelbrot ();

const int W = 800;
const int H = 800;
const float MAXR2 = 100;
const int MAXITER = 256;

int main ()
{
    txCreateWindow (W, H);

    DrawMandelbrot ();

    return 0;
}

void DrawMandelbrot ()
{
    float minY = -1  , maxY = 1;
    float minX = -1.5, maxX = 0.5;
    
    float dx = (maxX - minX) / W;
    float dy = (maxY - minY) / H;

    float x2 [8] = {};
    float y2 [8] = {};
    float xy [8] = {};

    float x    = 0, y    = 0;
    int   xpix = 0, ypix = 0;
    
    int last_time = clock ();
    int      time = 0;

    int i = 0;

    char fps_str [64] = "";

    while (!txGetAsyncKeyState (VK_ESCAPE))
    {
        for (ypix = 0, y = minY; ypix < H ; ypix++, y += dy)
        {
            for (xpix = 0, x = minX; xpix < W; xpix += 8, x += dx * 8)
            {
                float x0    [8] = {x, x + dx, x + dx * 2, x + dx * 3, x + dx * 4, x + dx * 5, x + dx * 6, x + dx * 7};
                float xnext [8] = {x, x + dx, x + dx * 2, x + dx * 3, x + dx * 4, x + dx * 5, x + dx * 6, x + dx * 7};
                float y0    [8] = {y, y, y, y, y, y, y, y};
                float ynext [8] = {y, y, y, y, y, y, y, y};

                int count [8] = {-1, -1, -1, -1, -1, -1, -1, -1};

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

                for (i = 0; i < 8; i++)
                {
                    if (count [i] == -1) txSetPixel (xpix + i, ypix, RGB (0, 0, 0));
                    else                 txSetPixel (xpix + i, ypix, RGB (count [i] % 256, 0, count [i] % 256));
                }
            }
        }
        time = clock () - last_time;
        last_time = clock ();
        sprintf (fps_str, "%.2lf", 1000. / time);
        txTextOut (W - 60, H - 20, fps_str);
    }
}
