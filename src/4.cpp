#include "mandelbrot.h"

void get_img (sf::Image &img)
{
    float x    = 0, y    = 0;
    int   xpix = 0, ypix = 0;

    int i = 0;

    for (ypix = 0, y = minY; ypix < H ; ypix++, y += dy)
    {
        for (xpix = 0, x = minX; xpix < W; xpix += 8, x += dx * 8)
        {
            int8 count = {};

            for (int rep = 0; rep < REP_NUM; rep ++)
            {
                __m256 x0 = _mm256_add_ps (_mm256_set1_ps (x), dx_0to7);
                __m256 xnext = x0;
                __m256 y0 = _mm256_set1_ps (y);
                __m256 ynext = y0;
                count.m256i = EIGHT_ZEROES_I;    

                int out_mask  = 0;
                __m256  out   = EIGHT_ZEROES_PS;
                __m256i iter8 = EIGHT_ZEROES_I;

                for (int iter = 0; iter < MAXITER; iter++)
                {
                    __m256 x2 = _mm256_mul_ps (xnext, xnext);
                    __m256 y2 = _mm256_mul_ps (ynext, ynext);
                    __m256 xy = _mm256_mul_ps (xnext, ynext);
                    __m256 r2 = _mm256_add_ps (x2, y2);

                    __m256 out_this_iter = _mm256_andnot_ps (out, _mm256_cmp_ps (r2, MAXR2_256, _CMP_GE_OQ));

                    count.m256i = _mm256_or_si256 (count.m256i, _mm256_and_si256 (iter8, _mm256_castps_si256 (out_this_iter)));

                    out       = _mm256_or_ps (out,  out_this_iter);
                    out_mask |= _mm256_movemask_ps (out_this_iter);
                        
                    if (out_mask == 0b11111111) break;

                    xnext = _mm256_add_ps (_mm256_sub_ps (x2, y2), x0);
                    ynext = _mm256_add_ps (_mm256_add_ps (xy, xy), y0);

                    iter8 = _mm256_add_epi32 (iter8, EIGHT_ONES_I);
                }
            }

            #ifdef _DRAW
            for (i = 0; i < 8; i++)
            {
                img.setPixel (xpix + 7 - i, ypix, sf::Color (count.arr [i] % 256, 0, count.arr [i] % 256 / 2));
            }
            #endif
        }
    }
}