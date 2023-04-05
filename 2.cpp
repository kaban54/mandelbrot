#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void DrawMandelbrot ();

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

    float x    = 0, y    = 0;
    int   xpix = 0, ypix = 0;

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
