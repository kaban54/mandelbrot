#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void DrawMandelbrot ();

const int W = 600;
const int H = 600;
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
    
    float x2 = 0;
    float y2 = 0;
    float xy = 0;

    float x0   = 0, y0   = 0;
    int   xpix = 0, ypix = 0;
    
    int last_time = clock ();
    int time = 0;

    char fps_str [64] = "";

    while (!txGetAsyncKeyState (VK_ESCAPE))
    {
        for (ypix = 0, y0 = minY; ypix < H ; ypix++, y0 += dy)
        {
            for (xpix = 0, x0 = minX; xpix < W; xpix++, x0 += dx)
            {
                float x = x0;
                float y = y0;
                int count = 0;

                for (count = 0; count < MAXITER; count++)
                {
                    x2 = x * x;
                    y2 = y * y;
                    xy = x * y;

                    if (x2 + y2 > MAXR2) break;

                    x = x2 - y2 + x0;
                    y = 2 * xy  + y0;
                }

                if (count == MAXITER) txSetPixel (xpix, ypix, RGB (0, 0, 0));
                else                  txSetPixel (xpix, ypix, RGB (count % 256, 0, count % 256));
            }
        }
        time = clock () - last_time;
        last_time = clock ();
        sprintf (fps_str, "%.2lf", 1000. / time);
        txTextOut (W - 60, H - 20, fps_str);
    }
}