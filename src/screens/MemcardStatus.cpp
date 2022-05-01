
#include "MemcardStatus.hpp"

#include "../Hyperblock.hpp"
#include "../Pad.hpp"

#include "Screens.hpp"

#include <stdio.h>
#include <debug.h>
#include <libmc.h>
#include <string.h>

MemcardStatus* g_MemcardStatus;

void MemcardStatus::Update()
{
    mcGetInfo(port, 0, &status.type, &status.free, &status.format);
    mcSync(0, NULL, &status.ret);

    mcGetInfo(port, 0, &status.type, &status.free, &status.format);
    mcSync(0, NULL, &status.ret);
}

void MemcardStatus::Init()
{
    //Update();
    status.ret = -1;
    status.type = 0;
    status.free = 0;
    status.format = 0;
    Render();
}

void MemcardStatus::Tick()
{
    if (Pad::Down())
    {
        if (++port >= 2)
        {
            port = 1;
        }

        Render();
    }

    if (Pad::Up())
    {
        if (--port <= 0)
        {
            port = 0;
        }

        Render();
    }

    if (Pad::Circle())
    {
        Update();
        Render();
    }

    if (Pad::Cross())
    {
        Hyperblock::ChangeScreen(Screen::MAIN_MENU);
    }
}

void MemcardStatus::Render()
{
    scr_clear();
    scr_printf("================================================================================\n");
    scr_printf("Hyperblock - Memory Card Status\n");
    scr_printf("================================================================================\n");
    scr_printf("O = Refresh | X = Back\n\n");

    scr_printf("%sMemory Card Port 0\n", (port == 0 ? "> " : ""));
    scr_printf("%sMemory Card Port 1\n\n", (port == 1 ? "> " : ""));
    
    if (status.ret == 0)
    {
        scr_printf("Memory card detected (%s)\n", (status.format ? "Formatted" : "Unformatted"));
    }
    else
    {
        scr_printf("No memory card detected\n");
    }
}
