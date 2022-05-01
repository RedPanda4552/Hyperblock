
#include "Hyperblock.hpp"
#include "Pad.hpp"

#include "screens/MainMenu.hpp"
#include "screens/MemcardStatus.hpp"
#include "screens/MemcardFormat.hpp"
#include "screens/FileWrite.hpp"

#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <stdio.h>
#include <debug.h>
#include <graph.h>
#include <libmc.h>

Screen currentScreen = Screen::MAIN_MENU;
Screen lastScreen;

void Hyperblock::ChangeScreen(Screen screen)
{
    currentScreen = screen;
}

void LoadModules()
{
    printf("LoadModules()\n");
    int ret = SifLoadModule("rom0:SIO2MAN", 0, NULL);

    if (ret < 0)
    {
        printf("SifLoadModule SIO2MAN failed: %d\n", ret);
        SleepThread();
    }

    ret = SifLoadModule("rom0:PADMAN", 0, NULL);

    if (ret < 0)
    {
        printf("SifLoadModule PADMAN failed: %d\n", ret);
        SleepThread();
    }

    ret = SifLoadModule("rom0:MCMAN", 0, NULL);
	
    if (ret < 0)
    {
		printf("SifLoadModule MCMAN failed: %d\n", ret);
		SleepThread();
	}

	ret = SifLoadModule("rom0:MCSERV", 0, NULL);
	
    if (ret < 0)
    {
		printf("SifLoadModule MCSERV failed: %d\n", ret);
		SleepThread();
	}
}

int main()
{
    printf("main()\n");
    g_MemcardStatus = new MemcardStatus();
    g_MemcardFormat = new MemcardFormat();
    g_FileWrite = new FileWrite();

    SifInitRpc(0);
    LoadModules();
    init_scr();
    scr_setCursor(0);
    Pad::Init();

    if (mcInit(MC_TYPE_MC) < 0)
    {
		printf("mcInit failed, stalling...\n");
		SleepThread();
	}

    while (true)
    {
        Pad::Poll();
        
        switch (currentScreen)
        {
            case Screen::MC_STATUS:
                if (lastScreen != Screen::MC_STATUS)
                {
                    g_MemcardStatus->Init();
                    lastScreen = currentScreen;
                }

                g_MemcardStatus->Tick();
                break;
            case Screen::MC_FORMAT:
                if (lastScreen != Screen::MC_FORMAT)
                {
                    g_MemcardFormat->Init();
                    lastScreen = currentScreen;
                }

                g_MemcardFormat->Tick();
                break;
            case Screen::FILE_WRITE:
                if (lastScreen != Screen::FILE_WRITE)
                {
                    g_FileWrite->Init();
                    lastScreen = currentScreen;
                }

                g_FileWrite->Tick();
                break;
            case Screen::EXIT:
                Exit(0);
                break;
            case Screen::MAIN_MENU:
                if (lastScreen != Screen::MAIN_MENU)
                {
                    MainMenu::Init();
                    lastScreen = currentScreen;
                }

                MainMenu::Tick();
                break;
            default:
                scr_printf("Illegal state reached, halting");
                SleepThread();
                return -10;
        }

        graph_wait_vsync();
    }

    return 0;
}
