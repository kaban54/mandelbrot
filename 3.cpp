#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void DrawMandelbrot ();

inline void mm_mov_ps1 (float *x, float val) {for (int i = 0; i < 8; i++) x [i]  = val  ; }
inline void mm_add_ps1 (float *x, float val) {for (int i = 0; i < 8; i++) x [i] += val  ; }
inline void mm_add_ps  (float *x, float *y ) {for (int i = 0; i < 8; i++) x [i] += y [i]; }
inline void mm_sub_ps  (float *x, float *y ) {for (int i = 0; i < 8; i++) x [i] -= y [i]; }
inline void mm_mul_ps  (float *x, float *y ) {for (int i = 0; i < 8; i++) x [i] *= y [i]; }
inline void mm_mov_ps  (float *x, float *y ) {for (int i = 0; i < 8; i++) x [i]  = y [i]; }

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
    float coefs [8] = {0, 1, 2, 3, 4, 5, 6, 7};

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
                float x0    [8] = {};
                float xnext [8] = {};
                float y0    [8] = {};
                float ynext [8] = {};
                float r2    [8] = {};

                mm_mov_ps1 (x0, dx);
                mm_mul_ps  (x0, coefs);
                mm_add_ps1 (x0, x);
                mm_mov_ps  (xnext, x0);

                mm_mov_ps1 (y0, y);
                mm_mov_ps1 (ynext, y);

                int count [8] = {-1, -1, -1, -1, -1, -1, -1, -1};

                int out = 0;

                for (int iter = 0; iter < MAXITER; iter++)
                {
                    mm_mov_ps (x2, xnext);
                    mm_mul_ps (x2, x2);

                    mm_mov_ps (y2, ynext);
                    mm_mul_ps (y2, y2);
                    
                    mm_mov_ps (xy, xnext);
                    mm_mul_ps (xy, ynext);

                    mm_mov_ps (r2, x2);
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

                    mm_mov_ps (xnext, x0);
                    mm_add_ps (xnext, x2);
                    mm_sub_ps (xnext, y2);
                    
                    mm_mov_ps (ynext, y0);
                    mm_add_ps (ynext, xy);
                    mm_add_ps (ynext, xy);
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
