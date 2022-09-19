
#pragma once

#include <array>
#include <string>

enum class Screen
{
    MC_STATUS,
    MC_FORMAT,
    FILE_WRITE,
    SUPERBLOCK,
    EXIT,
    MAIN_MENU
};

const std::array<std::string, 5> screenNames =
{
    "Memory Card Status",
    "Memory Card Format",
    "Write a Test File",
    "Inspect Superblock",
    "Exit"
};
