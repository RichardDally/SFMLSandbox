#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>
#include "Menu.h"
#include "Game.h"
#include "Event.h"


int main()
{
    sf::VideoMode _1080p(1920, 1080);
    sf::VideoMode svga(800, 600);
    sf::RenderWindow window(_1080p, "SFML Sandbox");
    window.setFramerateLimit(30);

    std::vector<Event> eventQueue;

    Game game(window, eventQueue);
    tgui::Gui tgui(window);
    Menu menu(tgui, eventQueue);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
            tgui.handleEvent(event);
            game.handleEvent(event);
        }

        for (auto event : eventQueue)
        {
            switch (event)
            {
                case Event::QUIT:
                {
                    window.close();
                    break;
                }
                case Event::START_GAME:
                {
                    game.setVisible(true);
                    menu.setVisible(false);
                    break;
                }
            }
        }
        eventQueue.clear();

        window.clear();
        game.draw();
        tgui.draw();
        window.display();
    }

    return EXIT_SUCCESS;
}
