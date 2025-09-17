#define _CRT_SECURE_NO_WARNINGS
//#define TEST_MODE

#include <iostream>
#include <fstream>
#include "MazeEscape.h"
#include "PlayerData.h"
#include "EnemyData.h"
#include "Test.h"

int main()
{
#ifdef TEST_MODE
	Test test;
	test.TestBattle();
#endif // TEST_MODE

#ifndef TEST_MODE
	InitializeMaze();	// 미로 초기화(파일에서 불러오기)
	MazeEscapeRun();	// 게임 시작
	ClearMaze();		// 미로 초기화하면서 동적할당 했던 메모리 정리
#endif // !TEST_MODE    

    return 0;
}

const char* DirectoryName = ".\\Data\\";
int MazeWidth = 20;
int MazeHeight = 10;
int** Maze = nullptr;


/*
		① 미로 탈출 게임 수정하기
			- Actor 클래스에 ICanBattle 인터페이스를 추가하기
			 - 전투를 할 수 있다는 것을 나타내는 인터페이스
			  - 주요 함수
				- ApplyDamage(ICanBattle* InTarget);
				- TakeDamage(float InDamage);
			  - 전투 코드 수정
			- enum을 모두 enum class로 변경하기
			- Monster가 공격을 할 때 각 몬스터별로 다른 방식으로 공격하기

			- 전투 시작 시 랜덤한 몬스터가 등장
*/

void InitializeMaze()
{
	std::string Data;
	ReadMapFile("MapData.txt", Data);
	ParseMapData(Data);
}

void MazeEscapeRun()
{
	PlayerData Player;

	FindStartPosition(Player.CurrentPosition);

	printf("~~ Maze Escape ~~\n");

	while (Player.Health > 0)
	{
		PrintMaze(Player.CurrentPosition);

		if (IsEnd(Player.CurrentPosition))
		{
			printf("Congratulations! You have escaped the maze!\n");
			break;
		}

		int MoveFlags = PrintAvailableMoves(Player.CurrentPosition);
		MoveDirection Direction = GetMoveInput(MoveFlags);
		switch (Direction)
		{
		case MoveDirection::DirUp:
			Player.CurrentPosition.y--;
			break;
		case MoveDirection::DirDown:
			Player.CurrentPosition.y++;
			break;
		case MoveDirection::DirLeft:
			Player.CurrentPosition.x--;
			break;
		case MoveDirection::DirRight:
			Player.CurrentPosition.x++;
			break;
		case MoveDirection::DirNone:
		default:
			// Critical Error
			break;
		}

		MoveEventProcess(Player);
	}

	if (Player.Health >= 0)
	{
		// 게임 클리어!
	}
	else
	{
		// 게임 오버
	}
}

void ClearMaze()
{
	if (Maze != nullptr)
	{
		for (int i = 0; i < MazeHeight; i++)
		{
			delete[] Maze[i];
			Maze[i] = nullptr;
		}
		delete[] Maze;
		Maze = nullptr;
	}
}

bool ReadMapFile(const char* MapFileName, std::string& OutDataString)
{
	char FilePath[256];
	strcpy(FilePath, DirectoryName);
	strcat(FilePath, MapFileName);

	std::ifstream InputFile(FilePath);
	if (!InputFile.is_open())
	{
		printf("File open FAIL!\n");
		printf("[%s] Check file path.\n", FilePath);
		return false;
	}

	OutDataString = std::string((std::istreambuf_iterator<char>(InputFile)),
		std::istreambuf_iterator<char>());

	return true;
}

bool ParseMapData(std::string& DataString)
{
	char Source[512];
	char* Current = Source;
	strcpy(Source, DataString.c_str());

	char* LinePtr = GetLine(&Current);

	int SizeNumbers[2] = { 0 };
	ParseLineData(LinePtr, 2, SizeNumbers);

	MazeWidth = SizeNumbers[0];
	MazeHeight = SizeNumbers[1];

	Maze = new int* [MazeHeight];
	for (int y = 0; y < MazeHeight; y++)
	{
		Maze[y] = new int[MazeWidth];
	}

	int HeightIndex = 0;
	while ((*Current) != '\0')
	{
		LinePtr = GetLine(&Current);
		ParseLineData(LinePtr, MazeWidth, Maze[HeightIndex]);
		HeightIndex++;

		if (HeightIndex >= MazeHeight)
			break;
	}

	return true;
}

int FindCharIndex(const char* TargetString, const char TargetCharacter)
{
	int Index = 0;
	bool IsFind = false;
	while (TargetString[Index] != '\0')
	{
		if (TargetString[Index] == TargetCharacter)
		{
			IsFind = true;
			break;
		}
		Index++;
	}

	if (!IsFind)
	{
		Index = -1;
	}
	return Index;
}

char* GetLine(char** InOutSource)
{
	char* Result = (*InOutSource);
	int Index = FindCharIndex((*InOutSource), '\n');
	if (Index == -1)
	{
		(*InOutSource) = (*InOutSource) + FindCharIndex((*InOutSource), '\0');
	}
	else
	{
		*((*InOutSource) + Index) = '\0';
		(*InOutSource) = (*InOutSource) + Index + 1;
	}

	return Result;
}

void ParseLineData(const char* LineData, int ArraySize, int* OutArray)
{
	const char* LinePointer = LineData;
	int SizeIndex = 0;
	while ((*LinePointer) != '\0')
	{
		int Result = 0;
		while ((*LinePointer) != ',' && (*LinePointer) != '\0')
		{
			Result = Result * 10 + (*LinePointer) - '0';
			LinePointer++;
		}
		OutArray[SizeIndex] = Result;
		SizeIndex++;

		if (ArraySize <= SizeIndex)	
			break;

		if (*LinePointer < '0' || *LinePointer > '9')
			LinePointer++;
	}
}

void PrintMaze(Position& position)
{
	for (int y = 0; y < MazeHeight; y++)
	{
		for (int x = 0; x < MazeWidth; x++)
		{
			if (position.x == x && position.y == y)
			{
				printf("P ");
			}
			else if (Maze[y][x] == MazeTile::Wall)
			{
				printf("# ");
			}
			else if (Maze[y][x] == MazeTile::Path)
			{
				printf(". ");
			}
			else if (Maze[y][x] == MazeTile::Start)
			{
				printf("S ");
			}
			else if (Maze[y][x] == MazeTile::End)
			{
				printf("E ");
			}
			else
			{
				// Critical Error. Map Data is Wrong.
			}
		}
		printf("\n");
	}
}

void FindStartPosition(Position& OutPosition)
{
	for (int y = 0; y < MazeHeight; y++)
	{
		for (int x = 0; x < MazeWidth; x++)
		{
			if (Maze[y][x] == MazeTile::Start)
			{
				OutPosition.x = x;
				OutPosition.y = y;
				return;
			}
		}
	}
	OutPosition.x = 0;
	OutPosition.y = 0;
}

int PrintAvailableMoves(Position& position)
{
	int MoveFlags = static_cast<int>(MoveDirection::DirNone);

	printf("이동할 수 있는 방향을 선택하세요 (w:위 a:왼쪽 s:아래쪽 d:오른쪽) : \n");
	if (!IsWall(position.x, position.y - 1))
	{
		printf("W(↑) ");
		MoveFlags |= MoveDirection::DirUp;
	}
	if (!IsWall(position.x, position.y + 1))
	{
		printf("S(↓) ");
		MoveFlags |= MoveDirection::DirDown;
	}
	if (!IsWall(position.x - 1, position.y))
	{
		printf("A(←) ");
		MoveFlags |= MoveDirection::DirLeft;
	}
	if (!IsWall(position.x + 1, position.y))
	{
		printf("D(→) ");
		MoveFlags |= MoveDirection::DirRight;
	}
	printf("\n");

	return MoveFlags;
}

bool IsWall(int X, int Y)
{
	bool isWall = false;
	if (Y < 0 || Y >= MazeHeight ||
		X < 0 || X >= MazeWidth ||
		Maze[Y][X] == MazeTile::Wall)
		isWall = true;
	return isWall;
}

bool IsEnd(Position& position)
{
	return Maze[position.y][position.x] == MazeTile::End;
}

MoveDirection GetMoveInput(int MoveFlags)
{
	char InputChar = 0;
	MoveDirection Direction = MoveDirection::DirNone;

	while (true)
	{
		printf("방향을 입력하세요 : ");
		std::cin >> InputChar;

		if ((InputChar == 'w' || InputChar == 'W')
			&& (MoveFlags & MoveDirection::DirUp) /*!= 0*/)
		{
			Direction = MoveDirection::DirUp;
			break;
		}
		if ((InputChar == 's' || InputChar == 'S')
			&& (MoveFlags & MoveDirection::DirDown) /*!= 0*/)
		{
			Direction = MoveDirection::DirDown;
			break;
		}
		if ((InputChar == 'a' || InputChar == 'A')
			&& (MoveFlags & MoveDirection::DirLeft) /*!= 0*/)
		{
			Direction = MoveDirection::DirLeft;
			break;
		}
		if ((InputChar == 'd' || InputChar == 'D')
			&& (MoveFlags & MoveDirection::DirRight) /*!= 0*/)
		{
			Direction = MoveDirection::DirRight;
			break;
		}

		printf("잘못된 입력입니다. 이동할 수 있는 방향 중에서 선택하세요.\n");
	}

	return Direction;
}

void MoveEventProcess(PlayerData& Player)
{
	float RandomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); // 0.0f ~ 1.0f
	//printf("Random Value = %.2f\n", RandomValue);
	if (RandomValue < 0.2f)
	{
		printf("적을 발견했습니다. 플레이어는 용감하게 싸웠다.\n");
		BattleEvent(Player);
	}
	else if (RandomValue < 0.4f)
	{
		printf("힐러를 찾았습니다!\n");
		HealerEvent(Player);
	}
	else
	{
		printf("아무일도 안 일어났습니다.\n");
	}
}

void BattleEvent(PlayerData& Player)
{
    EnemyData Enemy;
    printf("전투 시작!\n");
    while (Player.Health > 0 && Enemy.Health > 0)
    {
        // APlayer attacks Enemy
        Enemy.Health -= Player.AttackPower;
        printf("플레이어의 공격! 적의 체력 : %.1f\n", Enemy.Health);
        if (Enemy.Health <= 0)
        {
            printf("적이 패배했다!\n");
			printf("플레이어는 %d 골드를 얻었다.\n", Enemy.DropGold);
			Player.Gold += Enemy.DropGold;
            break;
        }

        // Enemy attacks APlayer
        Player.Health -= Enemy.AttackPower;
        printf("적의 공격! 플레이어의 체력: %.1f\n", Player.Health);
        if (Player.Health <= 0)
        {
            printf("플레이어는 패배했습니다.\n");
        }
    }
}

void HealerEvent(PlayerData& Player)
{
    printf("힐러: 얼마의 골드를 지불해서 힐을 받을래?\n(현재 체력: %.1f, 현재 골드: %d)\n", Player.Health, Player.Gold);
    int payGold = -1;
	while (true)
    {
		if (std::cin.fail())
		{
			std::cin.clear(); // 에러 상태 초기화
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 버퍼 비우기
			printf("숫자를 입력해야 합니다. 다시 시도하세요.\n");
		}
		      if (payGold <= 0)
		      {
		          printf("넌 힐을 원하지 않는거지? 알았어. \n");
			break;
		      }
		      if (payGold > Player.Gold)
		      {
		          printf("넌 충분한 골드가 지금 없어.\n");
			continue;
		      }
		break;
    }
	if (payGold > 0)
	{
		float healAmount = static_cast<float>(payGold);
		float newHealth = Player.Health + healAmount;
		if (newHealth > Player.MaxHealth)
			newHealth = Player.MaxHealth;
		Player.Health = newHealth;
		Player.Gold -= payGold;
		printf("Healed!");
	}
	printf("현재 체력: %.1f, 남아있는 골드: %d\n", Player.Health, Player.Gold);
}
