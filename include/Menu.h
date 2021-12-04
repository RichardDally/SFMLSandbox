#pragma once

#include <functional>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <spdlog/spdlog.h>
#include "Event.h"


struct ButtonDetails
{
    std::string name;
    std::function<void(void)> callback;
};


struct Menu
{
    tgui::Gui& gui_;
    std::vector<Event>& eventQueue_;
    tgui::Group::Ptr current_{ nullptr };
    tgui::Group::Ptr title_{ nullptr };
    tgui::Group::Ptr training_{ nullptr };

    Menu(tgui::Gui& gui, std::vector<Event>& eventQueue);
    void setVisible(bool state);

    template<typename T>
    tgui::Group::Ptr CreatePage(T callbackDispatch)
    {
        constexpr int pixelsBetweenButtons{ 10 };
        constexpr int buttonWidth{ 100 };
        constexpr int buttonHeight{ 40 };

        constexpr int groupWidth{ 150 };
        const int groupHeight{ buttonHeight * static_cast<int>(callbackDispatch.size()) + pixelsBetweenButtons * (static_cast<int>(callbackDispatch.size()) - 1) };

        auto group = tgui::Group::create();
        group->setSize(groupWidth, groupHeight);
        group->setVisible(false);
        group->setPosition({ "50%", "50%" });
        gui_.add(group);

        tgui::Button::Ptr previousButton{ nullptr };
        for (auto& [name, callback] : callbackDispatch)
        {
            auto button = tgui::Button::create(name);
            if (previousButton)
            {
                button->setPosition({ tgui::bindLeft(previousButton), tgui::bindBottom(previousButton) + pixelsBetweenButtons });
            }
            button->onClick(callback);
            button->setSize(buttonWidth, buttonHeight);
            group->add(button);
            previousButton = button;
        }

        return group;
    }

    // Callbacks
    void ShowTitlePage();
    void ShowTrainingPage();
    void ShowOnlinePage();
    void ShowOptionsPage();
    void StartGame();
    void Exit();
};
