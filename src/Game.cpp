#include <array>
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Event.h"


Game::Game(sf::RenderWindow& window, std::vector<Event>& eventQueue)
    : window_(window),
      eventQueue_(eventQueue),
      camera_(sf::FloatRect(0, 0, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)))
{
    if (!texture_.loadFromFile("REAPER1_IDLE_BLINKING_RIGHT.png"))
    {
        throw std::runtime_error("Unable to load texture_");
    }

    // Creating some "zones_" to explore
    std::array<sf::Color, 6> colors{
        sf::Color::Red,
        sf::Color::Green,
        sf::Color::Blue,
        sf::Color::Yellow,
        sf::Color::Magenta,
        sf::Color::Cyan,
    };

    sf::RectangleShape zone;
    zone.setSize({ zoneWidth_, zoneHeight_ });
    for (int c = 0; c < columns_; ++c)
    {
        for (int r = 0; r < rows_; r++)
        {
            zone.setPosition(1 + zoneWidth_ * c, 1 + zoneHeight_ * r);
            zone.setOutlineColor(sf::Color::White);
            zone.setFillColor(colors.at(zones_.size()));
            zone.setOutlineThickness(1.0f);
            zones_.push_back(zone);
        }
    }

    character_.setTexture(texture_);
    character_.setTextureRect(sf::IntRect(0, 0, 150, 150));
}

void Game::handleEvent(sf::Event event)
{
    if (!running_)
    {
        return;
    }

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            eventQueue_.push_back(Event::TOGGLE_PAUSE);
        }

        if (paused_)
        {
            return;
        }

        if (event.key.code == sf::Keyboard::Left)
        {
            // Camera should NOT move if left boundary is visible.
            // Camera should NOT move if character is not at center of the zone
            const auto leftBoundaryIsNotVisible{ camera_.getCenter().x - (window_.getSize().x / 2) - defaultDistance_ >= 0 };
            const auto characterIsRightToCenter{ character_.getPosition().x <= totalWidth_ - zoneWidth_ / 2 };
            if (leftBoundaryIsNotVisible && characterIsRightToCenter)
            {
                camera_.move(-defaultDistance_, 0.f);
            }

            const auto crossingBoundary{ character_.getPosition().x - defaultDistance_ < 0 };
            if (!crossingBoundary)
            {
                character_.move(-defaultDistance_, 0.f);
            }
        }
        else if (event.key.code == sf::Keyboard::Up)
        {
            // Camera should NOT move if top boundary is visible.
            // Camera should NOT move if character is not at center of the zone
            const auto topBoundaryIsNotVisible{ camera_.getCenter().y - (window_.getSize().y / 2) - defaultDistance_ >= 0 };
            const auto characterIsBottomToCenter{ character_.getPosition().y <= totalHeight_ - zoneHeight_ / 2 };
            if (topBoundaryIsNotVisible && characterIsBottomToCenter)
            {
                camera_.move(0.f, -defaultDistance_);
            }

            const auto crossingBoundary{ character_.getPosition().y - defaultDistance_ < 0 };
            if (!crossingBoundary)
            {
                character_.move(0.f, -defaultDistance_);
            }
        }
        else if (event.key.code == sf::Keyboard::Down)
        {
            // Camera should NOT move if bottom boundary is visible.
            // Camera should NOT move if character is not at center of the zone
            const auto bottomBoundaryIsNotVisible{ camera_.getCenter().y + (window_.getSize().y / 2) + defaultDistance_ <= totalHeight_ };
            const auto characterIsTopToCenter{ character_.getPosition().y >= zoneHeight_ / 2 };
            if (bottomBoundaryIsNotVisible && characterIsTopToCenter)
            {
                camera_.move(0.f, defaultDistance_);
            }

            const auto crossingBoundary{ character_.getPosition().y + defaultDistance_ + character_.getTextureRect().width > totalHeight_ };
            if (!crossingBoundary)
            {
                character_.move(0.f, defaultDistance_);
            }
        }
        else if (event.key.code == sf::Keyboard::Right)
        {
            // Camera should NOT move if right boundary is visible.
            // Camera should NOT move if character is not at center of the zone
            const auto rightBoundaryIsNotVisible{ camera_.getCenter().x + (window_.getSize().x / 2) + defaultDistance_ <= totalWidth_ };
            const auto characterIsLeftToCenter{ character_.getPosition().x >= zoneWidth_ / 2 };
            if (rightBoundaryIsNotVisible && characterIsLeftToCenter)
            {
                camera_.move(defaultDistance_, 0.f);
            }

            const auto crossingBoundary{ character_.getPosition().x + defaultDistance_ + character_.getTextureRect().height > totalWidth_ };
            if (!crossingBoundary)
            {
                character_.move(defaultDistance_, 0.f);
            }
        }
    }
}

void Game::draw()
{
    if (!visible_)
    {
        return;
    }

    window_.setView(camera_);
    for (const auto& zone : zones_)
    {
        window_.draw(zone);
    }
    window_.draw(character_);
}

void Game::setVisible(bool state)
{
    visible_ = state;
}

void Game::togglePause()
{
    paused_ = !paused_;
    spdlog::info("Game is now [{}]", paused_ ? "paused" : "resumed");
}

void Game::starts()
{
    setVisible(true);
    paused_ = false;
    running_ = true;

    // Set character at center of window
    const float x{ (window_.getSize().x - character_.getTextureRect().width) / 2.f };
    const float y{ (window_.getSize().y - character_.getTextureRect().height) / 2.f };
    character_.setPosition(x, y);

    // Reset camera to center
    camera_.reset(sf::FloatRect(0, 0, static_cast<float>(window_.getSize().x), static_cast<float>(window_.getSize().y)));
}

void Game::stops()
{
    setVisible(false);
    paused_ = false;
    running_ = false;
}
