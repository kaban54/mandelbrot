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
    
    float x2 = 0;
    float y2 = 0;
    float xy = 0;

    float x0   = 0, y0   = 0;
    int   xpix = 0, ypix = 0;

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

                #ifdef _DRAW
                if (count == MAXITER) img.setPixel (xpix, ypix, sf::Color::Black);
                else                  img.setPixel (xpix, ypix, sf::Color (count % 256, 0, count % 256));
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