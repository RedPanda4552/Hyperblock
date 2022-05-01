
#include "FileWrite.hpp"

#include "../Hyperblock.hpp"
#include "../Pad.hpp"

#include <stdio.h>
#include <debug.h>
#include <libmc.h>
#include <cstring>
#include <kernel.h>

FileWrite* g_FileWrite;

void FileWrite::Write()
{
    rootChdirFail = false;
    mkdirFail = false;
    chdirFail = false;
    openFail = false;
    writeFail = false;
    flushFail = false;
    closeFail = false;

    int ret;
    
    mcChdir(port, 0, "/", buf);
    mcSync(0, NULL, &ret);

    if (ret == 0)
    {
        mcMkDir(port, 0, "hyperblock");
        mcSync(0, NULL, &ret);

        if (ret == 0 || ret == -4)
        {
            mcChdir(port, 0, "hyperblock", buf);
            mcSync(0, NULL, &ret);

            if (ret == 0)
            {
                int fd = mcOpen(port, 0, "test", 0x03 | 0x0200);
                mcSync(0, NULL, &ret);

                if (ret >= 0)
                {
                    char data[256] = {0};
                    memset(data, 0xff, 256);

                    mcWrite(fd, data, 256);
                    mcSync(0, NULL, &ret);
                    
                    if (ret >= 0)
                    {
                        mcFlush(fd);
                        mcSync(0, NULL, &ret);
                        
                        if (ret == 0)
                        {
                            mcClose(fd);
                            mcSync(0, NULL, &ret);

                            if (ret != 0)
                            {
                                closeFail = true;
                            }
                        }
                        else
                        {
                            flushFail = true;
                        }
                    }
                    else
                    {
                        writeFail = true;
                    }
                }
                else 
                {
                    openFail = true;
                }
            }
            else
            {
                chdirFail = true;
            }
        }
        else
        {
            mkdirFail = true;
        }
    }
    else
    {
        rootChdirFail = true;
    }
    
    result = ret;
    isWriting = false;
    Render();
}

void FileWrite::Init()
{
    Render();
}

void FileWrite::Tick()
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
        isWriting = true;
        Render();
        Write();
    }

    if (Pad::Cross())
    {
        Hyperblock::ChangeScreen(Screen::MAIN_MENU);
    }
}

void FileWrite::Render()
{
    scr_clear();
    scr_printf("================================================================================\n");
    scr_printf("Hyperblock - Write a Test File\n");
    scr_printf("================================================================================\n");

    if (!isWriting)
    {
        scr_printf("O = Write | X = Back\n\n");

        scr_printf("%sMemory Card Port 0\n", (port == 0 ? "> " : ""));
        scr_printf("%sMemory Card Port 1\n\n", (port == 1 ? "> " : ""));
        
        if (result != -1)
        {
            if (rootChdirFail)
            {
                scr_printf("mcChdir fail (root): %d\n", result);
            }
            else if (mkdirFail)
            {
                scr_printf("mcMkDir fail: %d\n", result);    
            }
            else if (chdirFail)
            {
                scr_printf("mcChdir fail: %d\n", result);    
            }
            else if (openFail)
            {
                scr_printf("mcOpen fail: %d\n", result);    
            }
            else if (writeFail)
            {
                scr_printf("mcWrite fail: %d\n", result);
            }
            else if (flushFail)
            {
                scr_printf("mcFlush fail: %d\n", result);
            }
            else if (closeFail)
            {
                scr_printf("mcClose fail: %d\n", result);
            }
            else 
            {
                scr_printf("Successfully wrote 256 byte file!\n");
            }
        }
    }
    else
    {
        scr_printf("Write in progress on memory card port %d...\n", port);
    }
}
