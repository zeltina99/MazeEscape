#pragma once
#include <string>
#include "Position.h"
#include "PlayerData.h"

enum class MazeTile : int
{
    Path = 0,
    Wall = 1,
    Start = 2,
    End = 3
};

enum class MoveDirection : int
{
    DirNone = 0,
    DirUp = 1 << 0,
    DirDown = 1 << 1,
    DirLeft = 1 << 2,
    DirRight = 1 << 3
};

void InitializeMaze();
void MazeEscapeRun();
void ClearMaze();

bool ReadMapFile(const char* MapFileName, std::string& OutDataString);
bool ParseMapData(std::string& DataString);

int FindCharIndex(const char* TargetString, const char TargetCharacter);
char* GetLine(char** InOutSource);
void ParseLineData(const char* LineData, int ArraySize, int* OutArray);

void GetSpawnMonter(int SpawnMonster);

void PrintMaze(Position& position);
void FindStartPosition(Position& OutPosition);
int PrintAvailableMoves(Position& position);
bool IsWall(int X, int Y);
bool IsEnd(Position& position);
MoveDirection GetMoveInput(int MoveFlags);

//void MoveEventProcess(PlayerData& Player);
//void BattleEvent(PlayerData& Player);
//void HealerEvent(PlayerData& Player);

inline MoveDirection operator&(int Left, MoveDirection Right)
{
    return static_cast<MoveDirection>((Left) & static_cast<int>(Right));
}

inline bool operator&&(bool Left, MoveDirection Right)
{
    return static_cast<bool>((Left) && static_cast<int>(Right));
}

inline int& operator|=(int& Left, MoveDirection Right)
{
    Left = Left | static_cast<int>(Right);
    return Left;
}

inline bool operator==(int Left, MazeTile Right)
{
    return static_cast<bool>((Left) == static_cast<int>(Right));
}

class MazeEscape
{
public:
    MazeEscape() = default;

    void BattelEvent();
    void HealerEvent();
    void MoveEventProcess();
    void MazeEscapeRun();

private:

};