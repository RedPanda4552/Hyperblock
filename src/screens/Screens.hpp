
#pragma once

#include <array>
#include <string>

enum class Screen
{
    MC_STATUS,
    MC_FORMAT,
    MC_UNFORMAT,
    FILE_WRITE,
    SUPERBLOCK,
    EXIT,
    MAIN_MENU
};

const std::array<std::string, 6> screenNames =
{
    "Memory Card Status",
    "Memory Card Format",
    "Memory Card Unformat",
    "Write a Test File",
    "Inspect Superblock",
    "Exit"
};
