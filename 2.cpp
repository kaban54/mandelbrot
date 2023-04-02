#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int GetCount (float x0, float y0);

const float W = 600;
const float H = 600;
const float MAXR2 = 100;
const int MAXITER = 256;

int main ()
{
    txCreateWindow (W, H);

    float minY = -1,
          maxY =  1,
          minX = -1.5,
          maxX = 0.5;
    
    float dx = (maxX - minX) / W,
          dy = (maxY - minY) / H;
    
    float y = 0,
          x = 0;

    float xpix = 0,
          ypix = 0;
    
    int last_time = clock ();
    int time = 0;

    int count = 0;

    char fps_str [64] = "";

    while (!txGetAsyncKeyState (VK_ESCAPE))
    {
        y = minY;
        for (ypix = 0; ypix < H ; ypix++)
        {
            x = minX;
            for (xpix = 0; xpix < W; xpix++)
            {
                count = GetCount (x, y);
                if (count == -1) txSetPixel (xpix, ypix, RGB (0, 0, 0));
                else             txSetPixel (xpix, ypix, RGB (count % 256, 0, count % 256));
                x += dx;
            }
            y += dy;
        }
        time = clock () - last_time;
        last_time = clock ();
        sprintf (fps_str, "%lf", 1000. / time);
        txTextOut (W - 40, H - 20, fps_str);
    }

    return 0;
}

int GetCount (float x0, float y0)
{
    float x = x0;
    float y = y0;
    float tempx = 0;
    float tempy = 0;
    int count = 0;

    for (count = 0; count < MAXITER; count++)
    {
        if (x * x + y * y > MAXR2) return count;

        tempx = (x * x) - (y * y) + x0;
        tempy = (2 * x * y) + y0;
        x = tempx;
        y = tempy;
    }

    return -1;
}
