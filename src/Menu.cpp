#include "BoutonDetails.h"
#include "Menu.h"


Menu::Menu(tgui::Gui& gui, std::vector<Event>& eventQueue)
    : gui_(gui),
      eventQueue_(eventQueue)
{
    const auto titleDispatch = std::array{
        ButtonDetails{"Training", std::function([this]() {ShowTrainingPage(); })},
        ButtonDetails{"Online", std::function([this]() {ShowOnlinePage(); })},
        ButtonDetails{"Options", std::function([this]() {ShowOptionsPage(); })},
        ButtonDetails{"Exit", std::function([this]() {Exit(); })},
    };

    title_ = CreatePage(titleDispatch);
    gui_.add(title_);

    const auto trainingDispatch = std::array{
        ButtonDetails{"Game", std::function([this]() {StartGame(); })},
        ButtonDetails{"Back", std::function([this]() {ShowTitlePage(); })},
    };

    training_ = CreatePage(trainingDispatch);
    gui_.add(training_);

    // Setup
    current_ = title_;
    current_->setVisible(true);
}

void Menu::setVisible(bool state)
{
    current_->setVisible(state);
}

void Menu::ShowTitlePage()
{
    if (title_)
    {
        current_->setVisible(false);
        current_ = title_;
        current_->setVisible(true);
    }
}

void Menu::ShowTrainingPage()
{
    if (training_)
    {
        current_->setVisible(false);
        current_ = training_;
        current_->setVisible(true);
    }
}

void Menu::ShowOnlinePage()
{
    spdlog::info("online");
}

void Menu::ShowOptionsPage()
{
    spdlog::info("options");
}

void Menu::StartGame()
{
    eventQueue_.push_back(Event::START_GAME);
}

void Menu::Exit()
{
    eventQueue_.push_back(Event::QUIT);
}