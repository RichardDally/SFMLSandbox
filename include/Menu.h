#pragma once

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/ComboBox.hpp>
#include <TGUI/Widgets/Slider.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/VerticalLayout.hpp>
#include <TGUI/Widgets/HorizontalLayout.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <spdlog/spdlog.h>
#include "Event.h"


struct Menu
{
    tgui::Gui& gui_;
    std::vector<Event>& eventQueue_;

    tgui::Container::Ptr current_{ nullptr };
    tgui::Container::Ptr previous_{ nullptr };

    // All widget children are grouped (one is displayed at a time, see current_)
    tgui::Container::Ptr title_{ nullptr };
    tgui::Container::Ptr options_{ nullptr };
    tgui::Container::Ptr training_{ nullptr };
    tgui::Container::Ptr pause_{ nullptr };

    // Convenience pointer to quickly access resolution
    tgui::ComboBox::Ptr resolutionBox_{ nullptr };
    tgui::Slider::Ptr volumeSlider_{ nullptr };
    tgui::CheckBox::Ptr muteCheckbox_{ nullptr };
    static constexpr int pixelsBetweenButtons{ 10 };

    Menu(tgui::Gui& gui, std::vector<Event>& eventQueue);
    void setVisible(bool state);
    void togglePause();

    template<typename T>
    static std::vector<tgui::Widget::Ptr> CreateButtonWidgets(T callbackDispatch)
    {
        std::vector<tgui::Widget::Ptr> result;
        constexpr int widgetWidth{ 200 };
        constexpr int widgetHeight{ 40 };
        for (auto& [name, callback] : callbackDispatch)
        {
            auto button = tgui::Button::create(name);
            button->onClick(callback);
            button->setSize(widgetWidth, widgetHeight);
            result.push_back(button);
        }
        return result;
    }

    static tgui::VerticalLayout::Ptr AlignVerticallyWidgets(const std::vector<tgui::Widget::Ptr>& widgets)
    {
        auto layout = tgui::VerticalLayout::create();
        layout->setVisible(false);
        layout->setOrigin(0.5f, 0.5f);
        layout->setPosition({ "50%", "50%" });

        auto containerHeight{ 0.f };
        auto largestWidth{ 0.f };
        for (auto widget : widgets)
        {
            if (widget->getSize().x > largestWidth)
            {
                largestWidth = widget->getSize().x;
            }
            containerHeight += widget->getSize().y;
            layout->add(widget);
        }

        // Set size according to its widgets
        layout->setSize(largestWidth, containerHeight);

        return layout;
    }

    void CreateTitleGroup();
    void CreateTrainingGroup();
    void CreateOptionsGroup();
    void CreatePauseGroup();

    // Shared callback
    void BackToPreviousGroup();

    // Menu callbacks
    void ShowTitlePage();
    void ShowTrainingPage();
    void ShowOptionsPage();
    void StartGame();
    void Exit();

    // Options callbacks
    void ChangeResolution();
    void ChangeVolume();
    void MuteVolume();

    // Pause callbacks
    void ResumeGame();
    void BackToMainMenu();
};
