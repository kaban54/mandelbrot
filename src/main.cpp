#include "mandelbrot.h"

int main ()
{
    DrawMandelbrot ();
    return 0;
}

void DrawMandelbrot ()
{
    sf::Clock fps_clk;

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

    sf::RenderWindow window(sf::VideoMode(W, H), "eeeeeeee");

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

        fps_clk.restart ();

        get_img (img);
        
        sprintf (fps_str, "%.3f", 1 / fps_clk.restart().asSeconds());
        fps.setString (fps_str);

        texture.loadFromImage (img);
        sprite.setTexture (texture);

        window.clear ();
        window.draw (sprite);
        window.draw (fps);
        window.display ();
    }
}