#include <SFML/Graphics.hpp>


int main()
{
    sf::VideoMode _1080p(1920, 1080);
    sf::VideoMode svga(800, 600);
    sf::RenderWindow window(_1080p, "SFML window");
    window.setFramerateLimit(30);

    // Setup camera that will follow the character
    sf::Vector2f size = static_cast<sf::Vector2f>(window.getSize());
    sf::View camera(sf::FloatRect(0, 0, size.x, size.y));

    sf::Texture texture;
    if (!texture.loadFromFile("REAPER1_IDLE_BLINKING_RIGHT.png"))
        return EXIT_FAILURE;

    // Creating some "zones" to explore
    std::vector<sf::RectangleShape> zones;
    sf::RectangleShape zone;

    auto zoneWidth{ 1080.f };
    auto zoneHeight{ 1080.f };

    zone.setSize({ zoneWidth, zoneHeight });
    zone.setPosition(1, 1);
    zone.setOutlineColor(sf::Color::White);
    zone.setFillColor(sf::Color::Blue);
    zone.setOutlineThickness(1.0f);
    zones.push_back(zone);

    zone.setSize({ zoneWidth, zoneHeight });
    zone.setPosition(1 + zoneWidth, 1);
    zone.setOutlineColor(sf::Color::White);
    zone.setFillColor(sf::Color::Magenta);
    zone.setOutlineThickness(1.0f);
    zones.push_back(zone);

    zone.setSize({ zoneWidth, zoneHeight });
    zone.setPosition(1 + zoneWidth * 2, 1);
    zone.setOutlineColor(sf::Color::White);
    zone.setFillColor(sf::Color::Green);
    zone.setOutlineThickness(1.0f);
    zones.push_back(zone);

    zone.setSize({ zoneWidth, zoneHeight });
    zone.setPosition(1 + zoneWidth * 3, 1);
    zone.setOutlineColor(sf::Color::White);
    zone.setFillColor(sf::Color::Yellow);
    zone.setOutlineThickness(1.0f);
    zones.push_back(zone);

    sf::Sprite character;
    character.setTexture(texture);
    character.setTextureRect(sf::IntRect(0, 0, 150, 150));
    constexpr float defaultDistance = 10.f;

    // Set character at center of window
    const float x{ (window.getSize().x - character.getTextureRect().width) / 2.f };
    const float y{ (window.getSize().y - character.getTextureRect().height) / 2.f };
    character.move(x, y);

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
                    camera.move(-defaultDistance, 0.f);
                    character.move(-defaultDistance, 0.f);
                }
                else if (event.key.code == sf::Keyboard::Up)
                {
                    camera.move(0.f, -defaultDistance);
                    character.move(0.f, -defaultDistance);
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    camera.move(0.f, defaultDistance);
                    character.move(0.f, defaultDistance);
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    camera.move(defaultDistance, 0.f);
                    character.move(defaultDistance, 0.f);
                }
            }
            else if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        window.setView(camera);
        for (const auto& zone : zones)
        {
            window.draw(zone);
        }
        window.draw(character);
        window.display();
    }

    return EXIT_SUCCESS;
}
