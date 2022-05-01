
#pragma once

#include <string>
#include <libmc.h>
#include <array>

static constexpr size_t MAX_ENTRIES = 64;

static const std::array<std::string, 6> PROTECTED_ENTRIES = 
{
    "APPS",
    "BAEXEC-SYSTEM",
    "BEEXEC-SYSTEM",
    "BOOT",
    "OPL",
    "SYS-CONF"
};

class MemcardFormat
{
private:
    sceMcTblGetDir mcDir[MAX_ENTRIES];
    char buf[512];
    bool isFormatting = false;
    bool isProtected = false;
    int result = -1;
    int port = 0;

    void Format();
    void Render();

public:
    void Init();
    void Tick();
};

extern MemcardFormat* g_MemcardFormat;
