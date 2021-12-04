#include "Menu.h"
#include "Pause.h"
#include "BoutonDetails.h"


Pause::Pause(tgui::Gui& gui, std::vector<Event>& eventQueue)
    : gui_(gui),
      eventQueue_(eventQueue)
{
    const auto pauseDispatch = std::array{
        ButtonDetails{"Resume", std::function([this]() {ResumeCB(); })},
        ButtonDetails{"Options", std::function([this]() {OptionsCB(); })},
        ButtonDetails{"Main menu", std::function([this]() {MainMenuCB(); })},
        ButtonDetails{"Quit to desktop", std::function([this]() {QuitCB(); })},
    };

    current_ = Menu::CreateGroup(pauseDispatch);
    gui_.add(current_);
}

void Pause::toggleVisibility()
{
    current_->setVisible(!current_->isVisible());
}

void Pause::ResumeCB()
{
    eventQueue_.push_back(Event::TOGGLE_PAUSE);
}

void Pause::OptionsCB()
{
}

void Pause::MainMenuCB()
{
    eventQueue_.push_back(Event::BACK_TO_MAIN_MENU);
}

void Pause::QuitCB()
{
    eventQueue_.push_back(Event::QUIT);
}
