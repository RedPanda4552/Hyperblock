
#pragma once

class FileWrite
{
private:
    char buf[512];

    int port = 0;
    int result = -1;
    bool isWriting = false;
    
    bool rootChdirFail = false;
    bool mkdirFail = false;
    bool chdirFail = false;
    bool openFail = false;
    bool writeFail = false;
    bool flushFail = false;
    bool closeFail = false;

    void Write();
    void Render();
    
public:
    void Init();
    void Tick();
};

extern FileWrite* g_FileWrite;
