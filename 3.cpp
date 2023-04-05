#include <SFML/Graphics.hpp>
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

const char font_file_name [] = "Minecraft-Regular.ttf";
const size_t BUFSIZE = 64;

const int W = 800;
const int H = 800;
const float MAXR2 = 100;
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

    float x2 [8] = {};
    float y2 [8] = {};
    float xy [8] = {};
    float coefs [8] = {0, 1, 2, 3, 4, 5, 6, 7};

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

                #ifdef _DRAW
                for (i = 0; i < 8; i++)
                {
                    if (count [i] == -1) img.setPixel (xpix + i, ypix, sf::Color::Black);
                    else                 img.setPixel (xpix + i, ypix, sf::Color (count [i] % 256, 0, count [i] % 256));
                }
                #endif
            }
        }
        
        frames_count++;
        
        if (clk.getElapsedTime().asSeconds() >= 1)
        {
            sprintf (fps_str, "%d", frames_count);
            fps.setString (fps_str);
            puts (fps_str);
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
