#include "BoutonDetails.h"
#include "Menu.h"



Menu::Menu(tgui::Gui& gui, std::vector<Event>& eventQueue)
    : gui_(gui),
      eventQueue_(eventQueue)
{
    CreateTitleGroup();
    CreateTrainingGroup();
    CreateOptionsGroup();
    CreatePauseGroup();
}

void Menu::CreateTitleGroup()
{
    const auto titleDispatch = std::array{
        ButtonDetails{"Training", std::function([this]() {ShowTrainingPage(); })},
        ButtonDetails{"Online", std::function([this]() {ShowOnlinePage(); })},
        ButtonDetails{"Options", std::function([this]() {ShowOptionsPage(); })},
        ButtonDetails{"Exit", std::function([this]() {Exit(); })},
    };

    title_ = CreateGroup(titleDispatch);
    title_->setVisible(true);

    // Menu displays title first
    current_ = title_;
    gui_.add(title_);
}

void Menu::CreateTrainingGroup()
{
    const auto trainingDispatch = std::array{
        ButtonDetails{"Game", std::function([this]() {StartGame(); })},
        ButtonDetails{"Back", std::function([this]() {BackToPreviousGroup(); })},
    };

    training_ = CreateGroup(trainingDispatch);
    gui_.add(training_);
}

void Menu::CreateOptionsGroup()
{
    options_ = tgui::Group::create();
    options_->setVisible(false);
    options_->setPosition({ "50%", "50%" });

    auto label = tgui::Label::create("Options");
    options_->add(label);

    resolutionBox_ = tgui::ComboBox::create();
    resolutionBox_->onItemSelect(std::function([this]() {ChangeResolution(); }));
    resolutionBox_->setPosition({ tgui::bindLeft(label), tgui::bindBottom(label) + pixelsBetweenButtons });
    resolutionBox_->addItem("800x600");
    resolutionBox_->addItem("1280x800");
    resolutionBox_->addItem("1920x1080");
    resolutionBox_->addItem("2560x1440");
    resolutionBox_->setSelectedItem("1920x1080");
    options_->add(resolutionBox_);

    auto backButton = tgui::Button::create("Back");
    backButton->setPosition({ tgui::bindLeft(resolutionBox_), tgui::bindBottom(resolutionBox_) + pixelsBetweenButtons });
    backButton->onClick(std::function([this]() {BackToPreviousGroup(); }));
    options_->add(backButton);

    gui_.add(options_);
}

void Menu::CreatePauseGroup()
{
    const auto pauseDispatch = std::array{
        ButtonDetails{"Resume", std::function([this]() {ResumeGame(); })},
        ButtonDetails{"Options", std::function([this]() {ShowOptionsPage(); })},
        ButtonDetails{"Main menu", std::function([this]() {BackToMainMenu(); })},
        ButtonDetails{"Quit to desktop", std::function([this]() {Exit(); })},
    };

    pause_ = Menu::CreateGroup(pauseDispatch);
    pause_->setVisible(false);

    gui_.add(pause_);
}

void Menu::setVisible(bool state)
{
    if (current_)
    {
        current_->setVisible(state);
    }
}

void Menu::togglePause()
{
    if (current_ == pause_)
    {
        // Unpause
        current_->setVisible(false);
        current_ = nullptr;
        previous_ = nullptr;
    }
    else
    {
        // pause
        if (current_)
        {
            current_->setVisible(false);
        }
        current_ = pause_;
        current_->setVisible(true);
    }
}

void Menu::BackToPreviousGroup()
{
    if (previous_)
    {
        if (current_)
        {
            current_->setVisible(false);
        }
        current_ = previous_;
        previous_ = nullptr;
        current_->setVisible(true);
    }
}

void Menu::ShowTitlePage()
{
    if (title_)
    {
        if (current_)
        {
            current_->setVisible(false);
        }
        current_ = title_;
        current_->setVisible(true);
    }
}

void Menu::ShowTrainingPage()
{
    if (training_)
    {
        if (current_)
        {
            previous_ = current_;
            current_->setVisible(false);
        }
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
    if (options_)
    {
        if (current_)
        {
            previous_ = current_;
            current_->setVisible(false);
        }
        current_ = options_;
        current_->setVisible(true);
    }
}

void Menu::StartGame()
{
    eventQueue_.push_back(Event::START_GAME);
}

void Menu::Exit()
{
    eventQueue_.push_back(Event::QUIT);
}

void Menu::ChangeResolution()
{
    const auto selectedResolution = resolutionBox_->getSelectedItem().toStdString();
    if (selectedResolution == "800x600")
    {
        eventQueue_.push_back(Event::CHANGE_RESOLUTION_TO_800_600);
    }
    else if (selectedResolution == "1280x800")
    {
        eventQueue_.push_back(Event::CHANGE_RESOLUTION_TO_1280_800);
    }
    else if (selectedResolution == "1920x1080")
    {
        eventQueue_.push_back(Event::CHANGE_RESOLUTION_TO_1920_1080);
    }
    else if (selectedResolution == "2560x1440")
    {
        eventQueue_.push_back(Event::CHANGE_RESOLUTION_TO_2560_1440);
    }
    else
    {
        spdlog::critical("Unknown resolution [{}]", selectedResolution);
    }
}

void Menu::ResumeGame()
{
    eventQueue_.push_back(Event::TOGGLE_PAUSE);
}

void Menu::BackToMainMenu()
{
    eventQueue_.push_back(Event::BACK_TO_MAIN_MENU);
}
