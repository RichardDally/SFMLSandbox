#pragma once

#include "Event.h"


struct Game
{
    sf::RenderWindow& window_;
    std::vector<Event>& eventQueue_;
    sf::View camera_;
    sf::Texture texture_;
    sf::Sprite character_;
    std::vector<sf::RectangleShape> zones_;
    bool visible_{ false };
    bool paused_{ false };
    bool running_{ false };

    // Tweak dimensions
    constexpr static auto zoneWidth_{ 1920.f };
    constexpr static auto zoneHeight_{ 1080.f };
    constexpr static auto rows_{ 2 };
    constexpr static auto columns_{ 2 };
    constexpr static auto totalWidth_{ zoneWidth_ * columns_ };
    constexpr static auto totalHeight_{ zoneHeight_ * rows_ };
    constexpr static float defaultDistance_ = 30.f;

    Game(sf::RenderWindow& window, std::vector<Event>& eventQueue);
    void handleEvent(sf::Event event);
    void draw();
    void setVisible(bool state);
    void togglePause();
    void starts();
    void stops();
};
