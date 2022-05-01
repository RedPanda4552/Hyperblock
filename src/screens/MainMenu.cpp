
#include "../Hyperblock.hpp"
#include "../Pad.hpp"

#include "Screens.hpp"
#include "MainMenu.hpp"

#include <debug.h>

int selection = 0;

void MainMenu::Init()
{
    printf("MainMenu::Init()\n");
    Render();
}

void MainMenu::Tick()
{
    if (Pad::Down())
    {
        if (++selection >= screenNames.size())
        {
            selection = screenNames.size() - 1;
        }

        Render();
    }

    if (Pad::Up())
    {
        if (--selection <= 0)
        {
            selection = 0;
        }

        Render();
    }

    if (Pad::Circle())
    {
        Hyperblock::ChangeScreen((Screen) selection);
    }
}

void MainMenu::Render()
{
    scr_clear();
    scr_printf("================================================================================\n");
    scr_printf("Hyperblock - Main Menu\n");
    scr_printf("================================================================================\n");
    scr_printf("O = Select/OK\n\n");
    
    for (unsigned int i = 0; i < screenNames.size(); i++)
    {
        scr_printf("%s%s\n", (selection == i ? "> " : ""), screenNames.at(i).c_str());
    }
}
