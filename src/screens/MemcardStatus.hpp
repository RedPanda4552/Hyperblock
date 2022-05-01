
#pragma once

#include <array>
#include <string>

struct Status
{
    int ret = -1;
    int type = 0;
    int free = 0;
    int format = 0;
};

class MemcardStatus
{
private:
    int port = 0;

    Status status;
    void Update();
    void Render();

public:
    void Init();
    void Tick();
};

extern MemcardStatus* g_MemcardStatus;
