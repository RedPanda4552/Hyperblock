
#pragma once

struct SuperblockData
{
    char magic[28];
    char version[12];
    short pageLength;
    short pagesPerCluster;
    short pagesPerBlock;
    short unk_2e;
    int clustersPerCard;
    int allocOffset;
    int allocEnd;
    int rootDirCluster;
    int backupBlock1;
    int backupBlock2;
    int unk_48;
    int unk_4c;
    int indirectFatClusterList[32];
    int badBlockList[32];
    char cardType;
    char cardFlags;
    short unk_153;
    int clusterSize;
    int fatEntriesPerCluster;
    int clustersPerBlock;
    int cardForm;
    int rootDirCluster2;
    int unk1;
    int unk2;
    int maxAllocatableClusters;
    int unk3;
    int unk4;
    int unk5;
    int unk_fill[32];
};

class Superblock
{
private:
    int port = 0;
    int res = 1;
    //char* buf = nullptr;
    SuperblockData* data = nullptr;
    char* magicStr = nullptr;

    void ReadSuperblock();
    void Render();
    void Exit();

public:
    void Init();
    void Tick();
};

extern Superblock* g_Superblock;
