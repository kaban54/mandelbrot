#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <immintrin.h>

void DrawMandelbrot ();

union ps8
{
    __m256 m256;
    float arr [8];
};

const char font_file_name [] = "Minecraft-Regular.ttf";
const size_t BUFSIZE = 64;

const int W = 800;
const int H = 800;
const __m256 MAXR2 = _mm256_set1_ps (100);
const int MAXITER = 256;

int main ()
{
    DrawMandelbrot ();
    return 0;
}

void DrawMandelbrot ()
{
    float minY = -1  , maxY = 1;
    float minX = -1.5, maxX = 0.5;
    
    float dx = (maxX - minX) / W;
    float dy = (maxY - minY) / H;

    __m256 dx_0to7 = _mm256_set_ps (0, dx, dx * 2, dx * 3, dx * 4, dx * 5, dx * 6, dx * 7);

    float x    = 0, y    = 0;
    int   xpix = 0, ypix = 0;
    
    int last_time = clock ();
    int      time = 0;

    int i = 0;
    
    sf::Clock clk;

    sf::Sprite sprite;
    sf::Texture texture;
    sf::Image img;
    img.create (W, H);

    sf::Font font;
    sf::Text fps;
    font.loadFromFile (font_file_name);
    fps.setPosition (0, 0);
    fps.setFont (font);
    fps.setFillColor (sf::Color::Green);
    fps.setCharacterSize (20);
    char fps_str [BUFSIZE] = "";

    int frames_count = 0;

    sf::RenderWindow window(sf::VideoMode(W, H), "eeeeeeee");
    clk.restart ();

    while (window.isOpen ())
    {
        sf::Event event;
        while (window.pollEvent (event))
        {
            if (event.type == sf::Event::Closed)
                window.close ();
            
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close ();
            }
        }

        for (ypix = 0, y = minY; ypix < H ; ypix++, y += dy)
        {
            for (xpix = 0, x = minX; xpix < W; xpix += 8, x += dx * 8)
            {
                __m256 x0 = _mm256_add_ps (_mm256_set1_ps (x), dx_0to7);
                __m256 xnext = x0;
                __m256 y0 = _mm256_set1_ps (y);
                __m256 ynext = y0;

                int count [8] = {-1, -1, -1, -1, -1, -1, -1, -1};

                int out = 0;

                for (int iter = 0; iter < MAXITER; iter++)
                {
                    __m256 x2 = _mm256_mul_ps (xnext, xnext);
                    __m256 y2 = _mm256_mul_ps (ynext, ynext);
                    ps8 r2 = {};
                    r2.m256 = _mm256_add_ps (x2, y2);

                    out |= _mm256_movemask_ps (_mm256_cmp_ps (r2.m256, MAXR2, _CMP_GE_OQ));

                    for (i = 0; i < 8; i++)
                    {
                        if (out & (1 << i) && count [i] == -1)
                        {
                            count [i] = iter;
                        }
                    }

                    if (out == 0b11111111) break;

                    __m256 xy = _mm256_mul_ps (xnext, ynext);
                    
                    xnext = _mm256_add_ps (_mm256_sub_ps (x2, y2), x0);
                    ynext = _mm256_add_ps (_mm256_add_ps (xy, xy), y0);
                    //sf::sleep (sf::Time (sf::Int64 (999999999999)));
                }

                #ifdef _DRAW
                for (i = 0; i < 8; i++)
                {
                    if (count [i] == -1) img.setPixel (xpix + 7 - i, ypix, sf::Color::Black);
                    else                 img.setPixel (xpix + 7 - i, ypix, sf::Color (count [i] % 256, 0, count [i] % 256));
                }
                #endif
            }
        }
        
        frames_count++;
        
        if (clk.getElapsedTime().asSeconds() >= 1)
        {
            sprintf (fps_str, "%d", frames_count);
            fps.setString (fps_str);
            //puts (fps_str);
            frames_count = 0;
            clk.restart();
        }

        texture.loadFromImage (img);
        sprite.setTexture (texture);

        window.clear ();
        window.draw (sprite);
        window.draw (fps);
        window.display ();
    }
}
