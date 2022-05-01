
#include "MemcardFormat.hpp"

#include "../Hyperblock.hpp"
#include "../Pad.hpp"

#include <stdio.h>
#include <debug.h>

#include <cstring>
#include <string>
#include <array>

MemcardFormat* g_MemcardFormat;

void MemcardFormat::Format()
{
    isProtected = false;

    mcGetDir(port, 0, "/*", 0, MAX_ENTRIES, mcDir);
    mcSync(0, NULL, &result);

    for (size_t i = 0; i < result; i++)
    {
        for (std::string str : PROTECTED_ENTRIES)
        {
            if (strcmp((const char*) mcDir[i].EntryName, str.c_str()) == 0)
            {
                isProtected = true;
                break;
            }
        }

        if (isProtected)
        {
            break;
        }
    }

    if (!isProtected)
    {
        mcFormat(port, 0);
        mcSync(0, NULL, &result);
    }
    
    isFormatting = false;
    Render();
}

void MemcardFormat::Init()
{
    Render();
}

void MemcardFormat::Tick()
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
        isFormatting = true;
        Render();
        Format();
    }

    if (Pad::Cross())
    {
        Hyperblock::ChangeScreen(Screen::MAIN_MENU);
    }
}

void MemcardFormat::Render()
{
    scr_clear();
    scr_printf("================================================================================\n");
    scr_printf("Hyperblock - Memory Card Format\n");
    scr_printf("================================================================================\n");

    if (!isFormatting)
    {
        scr_printf("O = Format | X = Back\n\n");

        scr_printf("%sMemory Card Port 0\n", (port == 0 ? "> " : ""));
        scr_printf("%sMemory Card Port 1\n\n", (port == 1 ? "> " : ""));
        
        if (result != -1)
        {
            if (!isProtected)
            {
                scr_printf("Format result: %s\n", (result == 0 ? "Success" : "Error"));
            }
            else
            {
                scr_printf("FreeMCBoot install detected, format aborted\n");
            }
        }
    }
    else
    {
        scr_printf("Format in progress on memory card port %d...\n", port);
    }
}
