
#pragma once

#include "../Constants.hpp"

#include <libmc.h>

class MemcardUnformat
{
private:
    static const size_t MAX_ENTRIES = 64;
    sceMcTblGetDir mcDir[MAX_ENTRIES];
    char buf[512];
    bool isUnformatting = false;
    bool isProtected = false;
    int result = -1;
    int port = 0;

    bool IsProtected();
    void Unformat();
    void Render();

public:
    void Init();
    void Tick();
};

extern MemcardUnformat* g_MemcardUnformat;
