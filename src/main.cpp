#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>


int main()
{
    sf::VideoMode _1080p(1920, 1080);
    sf::VideoMode svga(800, 600);
    sf::RenderWindow window(_1080p, "SFML Sandbox");
    window.setFramerateLimit(30);

    sf::Texture texture;
    if (!texture.loadFromFile("REAPER1_IDLE_BLINKING_RIGHT.png"))
        return EXIT_FAILURE;

    // Creating some "zones" to explore
    std::vector<sf::RectangleShape> zones;

    // Tweak dimensions
    constexpr auto zoneWidth{ 1920.f };
    constexpr auto zoneHeight{ 1080.f };
    constexpr auto rows{ 2 };
    constexpr auto columns{ 2 };
    constexpr auto totalWidth{ zoneWidth * columns };
    constexpr auto totalHeight{ zoneHeight * rows };

    std::array<sf::Color, 6> colors {
        sf::Color::Red,
        sf::Color::Green,
        sf::Color::Blue,
        sf::Color::Yellow,
        sf::Color::Magenta,
        sf::Color::Cyan,
    };

    sf::RectangleShape zone;
    zone.setSize({ zoneWidth, zoneHeight });
    for (int c = 0; c < columns; ++c)
    {
        for (int r = 0; r < rows; r++)
        {
            zone.setPosition(1 + zoneWidth * c, 1 + zoneHeight * r);
            zone.setOutlineColor(sf::Color::White);
            zone.setFillColor(colors.at(zones.size()));
            zone.setOutlineThickness(1.0f);
            zones.push_back(zone);
        }
    }

    sf::Sprite character;
    character.setTexture(texture);
    character.setTextureRect(sf::IntRect(0, 0, 150, 150));
    constexpr float defaultDistance = 30.f;

    // Set character at center of window
    const float x{ (window.getSize().x - character.getTextureRect().width) / 2.f };
    const float y{ (window.getSize().y - character.getTextureRect().height) / 2.f };
    character.move(x, y);

    // Setup camera that will follow the character
    sf::Vector2f size = static_cast<sf::Vector2f>(window.getSize());
    sf::View camera(sf::FloatRect(0, 0, size.x, size.y));

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
                    // Camera should NOT move if left boundary is visible.
                    // Camera should NOT move if character is not at center of the zone
                    const auto leftBoundaryIsNotVisible{ camera.getCenter().x - (size.x / 2) - defaultDistance >= 0 };
                    const auto characterIsRightToCenter{ character.getPosition().x <= totalWidth - zoneWidth / 2 };
                    if (leftBoundaryIsNotVisible && characterIsRightToCenter)
                    {
                        camera.move(-defaultDistance, 0.f);
                    }

                    const auto crossingBoundary{ character.getPosition().x - defaultDistance < 0 };
                    if (!crossingBoundary)
                    {
                        character.move(-defaultDistance, 0.f);
                    }
                }
                else if (event.key.code == sf::Keyboard::Up)
                {
                    // Camera should NOT move if top boundary is visible.
                    // Camera should NOT move if character is not at center of the zone
                    const auto topBoundaryIsNotVisible{ camera.getCenter().y - (size.y / 2) - defaultDistance >= 0 };
                    const auto characterIsBottomToCenter{ character.getPosition().y <= totalHeight - zoneHeight / 2 };
                    if (topBoundaryIsNotVisible && characterIsBottomToCenter)
                    {
                        camera.move(0.f, -defaultDistance);
                    }

                    const auto crossingBoundary{ character.getPosition().y - defaultDistance < 0 };
                    if (!crossingBoundary)
                    {
                        character.move(0.f, -defaultDistance);
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    // Camera should NOT move if bottom boundary is visible.
                    // Camera should NOT move if character is not at center of the zone
                    const auto bottomBoundaryIsNotVisible{ camera.getCenter().y + (size.y / 2) + defaultDistance <= totalHeight };
                    const auto characterIsTopToCenter{ character.getPosition().y >= zoneHeight / 2 };
                    if (bottomBoundaryIsNotVisible && characterIsTopToCenter)
                    {
                        camera.move(0.f, defaultDistance);
                    }

                    const auto crossingBoundary{ character.getPosition().y + defaultDistance + character.getTextureRect().width > totalHeight };
                    if (!crossingBoundary)
                    {
                        character.move(0.f, defaultDistance);
                    }
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    // Camera should NOT move if right boundary is visible.
                    // Camera should NOT move if character is not at center of the zone
                    const auto rightBoundaryIsNotVisible{ camera.getCenter().x + (size.x / 2) + defaultDistance <= totalWidth };
                    const auto characterIsLeftToCenter{ character.getPosition().x >= zoneWidth / 2 };
                    if (rightBoundaryIsNotVisible && characterIsLeftToCenter)
                    {
                        camera.move(defaultDistance, 0.f);
                    }

                    const auto crossingBoundary{ character.getPosition().x + defaultDistance + character.getTextureRect().height > totalWidth };
                    if (!crossingBoundary)
                    {
                        character.move(defaultDistance, 0.f);
                    }
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
