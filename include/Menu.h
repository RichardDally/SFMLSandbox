#pragma once

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/ComboBox.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <spdlog/spdlog.h>
#include "Event.h"


struct Menu
{
    tgui::Gui& gui_;
    std::vector<Event>& eventQueue_;
    tgui::Group::Ptr current_{ nullptr };
    tgui::Group::Ptr title_{ nullptr };
    tgui::Group::Ptr options_{ nullptr };
    tgui::Group::Ptr training_{ nullptr };
    tgui::ComboBox::Ptr resolutionBox_{ nullptr };
    static constexpr int pixelsBetweenButtons{ 10 };

    Menu(tgui::Gui& gui, std::vector<Event>& eventQueue);
    void setVisible(bool state);

    template<typename T>
    static tgui::Group::Ptr CreateGroup(T callbackDispatch)
    {
        constexpr int buttonWidth{ 170 };
        constexpr int buttonHeight{ 40 };

        constexpr int groupWidth{ 150 };
        const int groupHeight{ buttonHeight * static_cast<int>(callbackDispatch.size()) + pixelsBetweenButtons * (static_cast<int>(callbackDispatch.size()) - 1) };

        auto group = tgui::Group::create();
        group->setSize(groupWidth, groupHeight);
        group->setVisible(false);
        group->setPosition({ "50%", "50%" });

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

    void CreateTitleGroup();
    void CreateTrainingGroup();
    void CreateOptionsGroup();

    // Callbacks
    void ShowTitlePage();
    void ShowTrainingPage();
    void ShowOnlinePage();
    void ShowOptionsPage();
    void StartGame();
    void Exit();
    void ChangeResolution();
};
