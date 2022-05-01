
#include "Pad.hpp"

#include <kernel.h>
#include <stdio.h>

#include "libpad.h"

constexpr int port = 0;
constexpr int slot = 0;

char padBuf[256] __attribute__((aligned(64)));
struct padButtonStatus buttons;
unsigned int padState;
unsigned int maskedState;

int WaitPadReady()
{
    int state = padGetState(port, slot);

    while ((state != PAD_STATE_STABLE) && (state != PAD_STATE_FINDCTP1))
    {
        state = padGetState(port, slot);
    }

    return 0;
}

int Pad::Init()
{
    int ret;
    padInit(0);

    if ((ret = padPortOpen(port, slot, padBuf)) == 0)
    {
        printf("padOpenPort failed: %d\n", ret);
        SleepThread();
    }
    
    WaitPadReady();
    int modes = padInfoMode(port, slot, PAD_MODETABLE, -1);

    if (modes == 0)
    {
        return 1;
    }

    int i = 0;
    
    do
    {
        if (padInfoMode(port, slot, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK)
        {
            break;
        }
            
        i++;
    }
    while (i < modes);

    if (i >= modes)
    {
        return 1;
    }

    ret = padInfoMode(port, slot, PAD_MODECUREXID, 0);

    if (ret == 0)
    {
        return 1;
    }

    WaitPadReady();
    return 1;
}

void Pad::Poll()
{
    int ret = padGetState(port, slot);
        
    while ((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1))
    {
        ret = padGetState(port, slot);
    }

    ret = padRead(port, slot, &buttons);

    if (ret != 0)
    {
        const unsigned int newPadState = 0xffff ^ buttons.btns;
        maskedState = newPadState & ~padState;
        padState = newPadState;
    }
}

bool Pad::Circle()
{
    return maskedState & PAD_CIRCLE;
}

bool Pad::Cross()
{
    return maskedState & PAD_CROSS;
}

bool Pad::Up()
{
    return maskedState & PAD_UP;
}

bool Pad::Down()
{
    return maskedState & PAD_DOWN;
}

bool Pad::Left()
{
    return maskedState & PAD_LEFT;
}

bool Pad::Right()
{
    return maskedState & PAD_RIGHT;
}
