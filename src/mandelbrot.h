#ifndef _MANDELBROT_H
#define _MANDELBROT_H

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <immintrin.h>

#ifndef REP_NUM
#define REP_NUM 1
#endif


union ps8
{
    __m256 m256;
    float arr [8];
};

union int8
{
    __m256i m256i;
    int arr [8];
};


const char font_file_name [] = "Minecraft-Regular.ttf";
const size_t BUFSIZE = 64;

const int W = 800;
const int H = 800;
const float MAXR2 = 100;
const int MAXITER = 256;

const float minY = -1;
const float maxY =  1;
const float minX = -1.5;
const float maxX =  0.5;
    
const float dx = (maxX - minX) / W;
const float dy = (maxY - minY) / H;


const __m256 MAXR2_256 = _mm256_set1_ps (100);

const __m256i EIGHT_ONES_I    = _mm256_set1_epi32 (1);
const __m256i EIGHT_ZEROES_I  = _mm256_set1_epi32 (0);
const __m256  EIGHT_ZEROES_PS = _mm256_set1_ps    (0);
const __m256  ALL_OUT         = _mm256_castsi256_ps (_mm256_set1_epi32 (0xffffffff));
const __m256  dx_0to7         = _mm256_set_ps (0, dx, dx * 2, dx * 3, dx * 4, dx * 5, dx * 6, dx * 7);


void DrawMandelbrot ();

void get_img (sf::Image &img);

inline void mm_set_ps1 (float *x, float val) {for (int i = 0; i < 8; i++) x [i]  = val  ; }
inline void mm_add_ps1 (float *x, float val) {for (int i = 0; i < 8; i++) x [i] += val  ; }
inline void mm_add_ps  (float *x, float *y ) {for (int i = 0; i < 8; i++) x [i] += y [i]; }
inline void mm_sub_ps  (float *x, float *y ) {for (int i = 0; i < 8; i++) x [i] -= y [i]; }
inline void mm_mul_ps  (float *x, float *y ) {for (int i = 0; i < 8; i++) x [i] *= y [i]; }
inline void mm_cpy_ps  (float *x, float *y ) {for (int i = 0; i < 8; i++) x [i]  = y [i]; }
inline void mm_set_i1  (int   *x, int   val) {for (int i = 0; i < 8; i++) x [i]  = val  ; }

#endif