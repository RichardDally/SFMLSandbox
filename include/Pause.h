#pragma once

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <spdlog/spdlog.h>
#include "Event.h"


struct Pause
{
    tgui::Gui& gui_;
    std::vector<Event>& eventQueue_;
    tgui::Group::Ptr current_{ nullptr };

    Pause(tgui::Gui& gui, std::vector<Event>& eventQueue);
    void toggleVisibility();

    // Callbacks
    void ResumeCB();
    void OptionsCB();
    void MainMenuCB();
    void QuitCB();
};
