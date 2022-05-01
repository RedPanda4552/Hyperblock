
#pragma once

#include <array>
#include <string>

enum class Screen
{
    MC_STATUS,
    MC_FORMAT,
    FILE_WRITE,
    EXIT,
    MAIN_MENU
};

const std::array<std::string, 7> screenNames =
{
    "Memory Card Status",
    "Memory Card Format",
    "Write a Test File",
    "Exit"
};
