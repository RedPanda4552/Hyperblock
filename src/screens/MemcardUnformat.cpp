
#include "MemcardUnformat.hpp"

#include "../Hyperblock.hpp"
#include "../Pad.hpp"

#include <stdio.h>
#include <debug.h>

#include <cstring>
#include <string>
#include <array>

MemcardUnformat* g_MemcardUnformat;

bool MemcardUnformat::IsProtected()
{
    mcGetDir(port, 0, "/*", 0, MAX_ENTRIES, mcDir);
    mcSync(0, NULL, &result);

    for (size_t i = 0; i < result; i++)
    {
        for (std::string str : PROTECTED_ENTRIES)
        {
            if (strcmp((const char*) mcDir[i].EntryName, str.c_str()) == 0)
            {
                isProtected = true;
                return true;
            }
        }
    }

    isProtected = false;
    return false;
}

void MemcardUnformat::Unformat()
{
    if (!IsProtected())
    {
        mcUnformat(port, 0);
        mcSync(0, NULL, &result);
    }
    
    isUnformatting = false;
    Render();
}

void MemcardUnformat::Init()
{
    Render();
}

void MemcardUnformat::Tick()
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
        isUnformatting = true;
        Render();
        Unformat();
    }

    if (Pad::Cross())
    {
        Hyperblock::ChangeScreen(Screen::MAIN_MENU);
    }
}

void MemcardUnformat::Render()
{
    scr_clear();
    scr_printf("================================================================================\n");
    scr_printf("Hyperblock - Memory Card Unformat\n");
    scr_printf("================================================================================\n");

    if (!isUnformatting)
    {
        scr_printf("O = Unformat | X = Back\n\n");

        scr_printf("%sMemory Card Port 0\n", (port == 0 ? "> " : ""));
        scr_printf("%sMemory Card Port 1\n\n", (port == 1 ? "> " : ""));
        
        if (result != -1)
        {
            if (!isProtected)
            {
                scr_printf("Unformat result: %s\n", (result == 0 ? "Success" : "Error"));
            }
            else
            {
                scr_printf("FreeMCBoot install detected, unformat aborted\n");
            }
        }
    }
    else
    {
        scr_printf("Unformat in progress on memory card port %d...\n", port);
    }
}
