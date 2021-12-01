#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    sf::Texture texture;
    if (!texture.loadFromFile("REAPER1_IDLE_BLINKING_RIGHT.png"))
        return EXIT_FAILURE;

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 150, 150));
    constexpr float defaultDistance = 5.f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::Left)
                {
                    sprite.move(-defaultDistance, 0.f);
                }
                else if (event.key.code == sf::Keyboard::Up)
                {
                    sprite.move(0.f, -defaultDistance);
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    sprite.move(0.f, defaultDistance);
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    sprite.move(defaultDistance, 0.f);
                }
            }
            else if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return EXIT_SUCCESS;
}
