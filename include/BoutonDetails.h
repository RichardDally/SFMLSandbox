#pragma once

#include <string>
#include <functional>


struct ButtonDetails
{
    std::string name;
    std::function<void(void)> callback;
};
