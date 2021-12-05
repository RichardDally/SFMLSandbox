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
        ButtonDetails{"Options", std::function([this]() {ShowOptionsPage(); })},
        ButtonDetails{"Exit", std::function([this]() {Exit(); })},
    };

    const auto widgets = CreateButtonWidgets(titleDispatch);
    title_ = AlignVerticallyWidgets(widgets);
    gui_.add(title_);

    // Menu displays title first
    title_->setVisible(true);
    current_ = title_;
}

void Menu::CreateTrainingGroup()
{
    const auto trainingDispatch = std::array{
        ButtonDetails{"Game", std::function([this]() {StartGame(); })},
        ButtonDetails{"Back", std::function([this]() {BackToPreviousGroup(); })},
    };

    const auto widgets = CreateButtonWidgets(trainingDispatch);
    training_ = AlignVerticallyWidgets(widgets);

    gui_.add(training_);
}

void Menu::CreateOptionsGroup()
{
    auto optionsLabel = tgui::Label::create("Options");
    optionsLabel->setWidth(300);
    optionsLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);

    resolutionBox_ = tgui::ComboBox::create();
    resolutionBox_->onItemSelect(std::function([this]() { ChangeResolution(); }));
    resolutionBox_->addItem("800x600");
    resolutionBox_->addItem("1280x800");
    resolutionBox_->addItem("1920x1080");
    resolutionBox_->addItem("2560x1440");
    resolutionBox_->setSelectedItem("1920x1080");

    // Volume
    auto volumeLayout = tgui::HorizontalLayout::create();

    // Volume slider
    volumeSlider_ = tgui::Slider::create();
    volumeSlider_->setWidth(400);
    volumeSlider_->setMinimum(0.f);
    volumeSlider_->setMaximum(100.f);
    volumeSlider_->setStep(1.f);
    volumeSlider_->setValue(50.f);
    volumeSlider_->onValueChange(std::function([this]() { ChangeVolume(); }));

    // Volume optionsLabel
    auto volumeLabel = tgui::Label::create();
    volumeLabel->setWidth(100);
    volumeLabel->setText(fmt::format("{}", volumeSlider_->getValue()));

    // Mute checkbox
    muteCheckbox_ = tgui::CheckBox::create();
    muteCheckbox_->setWidth(10);
    muteCheckbox_->onChange(std::function([this]() { MuteVolume(); }));
    muteCheckbox_->setText("Mute");

    // Add widgets to layout
    volumeLayout->add(volumeSlider_);
    volumeLayout->add(volumeLabel);
    volumeLayout->add(muteCheckbox_);

    auto backButton = tgui::Button::create("Back");
    backButton->onClick(std::function([this]() { BackToPreviousGroup(); }));

    std::vector<tgui::Widget::Ptr> widgets;
    widgets.push_back(optionsLabel);
    widgets.push_back(resolutionBox_);
    widgets.push_back(volumeLayout);
    widgets.push_back(backButton);

    options_ = AlignVerticallyWidgets(widgets);
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

    const auto widgets = CreateButtonWidgets(pauseDispatch);
    pause_ = AlignVerticallyWidgets(widgets);

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

void Menu::ChangeVolume()
{

}

void Menu::MuteVolume()
{

}

void Menu::ResumeGame()
{
    eventQueue_.push_back(Event::TOGGLE_PAUSE);
}

void Menu::BackToMainMenu()
{
    eventQueue_.push_back(Event::BACK_TO_MAIN_MENU);
}
