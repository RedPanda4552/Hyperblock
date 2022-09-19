
#include "Superblock.hpp"

#include "../Hyperblock.hpp"
#include "../Pad.hpp"

#include <stdio.h>
#include <debug.h>
#include <libmc.h>
#include <cstring>

Superblock* g_Superblock;

void Superblock::Init()
{
/*
    if (buf == nullptr)
    {
        buf = (char*) aligned_alloc(64, 512);
        memset(buf, 0, 512);
    }
*/    
    if (data == nullptr)
    {
        data = (SuperblockData*) aligned_alloc(64, 512);
        memset(data, 0, 512);
    }

    if (magicStr == nullptr)
    {
        magicStr = (char*) aligned_alloc(64, 32);
        memset(magicStr, '\0', 32);
    }
    
    Render();
}

void Superblock::Tick()
{
    if (Pad::Down())
    {
        if (++port >= 2)
        {
            port = 1;
        }

        res = 1;
        Render();
    }

    if (Pad::Up())
    {
        if (--port <= 0)
        {
            port = 0;
        }

        res = 1;
        Render();
    }

    if (Pad::Circle())
    {
        ReadSuperblock();
        //Render();
    }

    if (Pad::Cross())
    {
        Exit();
        Hyperblock::ChangeScreen(Screen::MAIN_MENU);
    }
}

void Superblock::ReadSuperblock()
{
    int _type = 0, _free = 0, _format = 0;
    mcGetInfo(port, 0, &_type, &_free, &_format);
    mcSync(0, NULL, &res);

    mcGetInfo(port, 0, &_type, &_free, &_format);
    mcSync(0, NULL, &res);

    void* buf = aligned_alloc(64, 512);
    mcReadPage(port, 0, 0, buf);
    mcSync(0, NULL, &res);

    if (res == 0)
    {
        memcpy(data, buf, sizeof(SuperblockData));
    }

    free(buf);
}

void Superblock::Render()
{
    scr_clear();
    scr_printf("================================================================================\n");
    scr_printf("Hyperblock - Inspect Superblock\n");
    scr_printf("================================================================================\n");

    scr_printf("O = Read | X = Back\n\n");

    scr_printf("%sMemory Card Port 0\n", (port == 0 ? "> " : ""));
    scr_printf("%sMemory Card Port 1\n\n", (port == 1 ? "> " : ""));
    scr_printf("sizeof = %d", sizeof(SuperblockData));

    if (res == 0)
    {
        scr_printf("--------------------------------------------------------------------------------");
        scr_printf("Superblock In Port %d\n", port);
        scr_printf("--------------------------------------------------------------------------------");
/*
        //memcpy(magicStr, data->magic, 28);
        //scr_printf("Magic Format String: %s\n", magicStr);
        scr_printf("Version: %s\n", data->version);
        scr_printf("Page Length (Bytes): %d / %04X\n", data->pageLength, data->pageLength);
        scr_printf("Pages Per Cluster: %d / %04X\n", data->pagesPerCluster, data->pagesPerCluster);
        scr_printf("Pages Per Block: %d / %04X\n", data->pagesPerBlock, data->pagesPerBlock);
        //scr_printf("Unknown (0x002E): %d / %04X\n", data->unk_2e, data->unk_2e);
        scr_printf("Clusters On Card: %d / %08X\n", data->clustersPerCard, data->clustersPerCard);
        scr_printf("Allocation Offset: %d / %08X\n", data->allocOffset, data->allocOffset);
        scr_printf("Allocation End: %d / %08X\n", data->allocEnd, data->allocEnd);
        scr_printf("Root Directory Cluster: %d / %08X\n", data->rootDirCluster, data->rootDirCluster);
        scr_printf("Backup Block 1: %d / %08X\n", data->backupBlock1, data->backupBlock1);
        scr_printf("Backup Block 2: %d / %08X\n", data->backupBlock2, data->backupBlock2);
        //scr_printf("Unknown (0x0048): %d / %08X\n", data->unk_48, data->unk_48);
        //scr_printf("Unknown (0x004c): %d / %08X\n", data->unk_4c, data->unk_4c);
        //scr_printf("Indirect FAT Cluster List: Not Yet Implemented\n");
        //scr_printf("Bad Block List: Not Yet Implemented\n");
        scr_printf("Card Type: %d / %02X\n", data->cardType, data->cardType);
        scr_printf("Card Flags: %d / %02X\n", data->cardFlags, data->cardFlags);
*/
    }
    else if (res < 0)
    {
        scr_printf("Failed to read superblock (%d / %08X)\n", res, res);
    }
    else
    {
        scr_printf(":modCheck:\n");
    }
}

void Superblock::Exit()
{
    port = 0;
    res = 1;
/*
    if (buf != nullptr)
    {
        free(buf);
        buf = nullptr;
    }
*/
    if (data != nullptr)
    {
        free(data);
        data = nullptr;
    }

    if (magicStr != nullptr)
    {
        free(magicStr);
        magicStr = nullptr;
    }
}
