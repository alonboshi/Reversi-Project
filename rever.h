#pragma once
#include <stdio.h>

#define GAMESIZE 8
#define MATRIXSIZE GAMESIZE+2
#define VECTORSIZE 8


typedef enum {
	UP,
	UPRIGHT,
	RIGHT,
	DOWNRIGHT,
	DOWN,
	DOWNLEFT,
	LEFT,
	UPLEFT
} Direction;

typedef enum Player
{
	X,
	O,
	EMPTY,
	POSIBBLEMOVE

}Player, Color;


typedef struct Vector
{
	int row, col;
}Vector;


int MainGame();

void ChangePlayer(Player* player);

void InitBoard(Color Board[][MATRIXSIZE]);

void ClearAllPossibleMoves(Color Board[][MATRIXSIZE]);

void InitVector(Vector vector[VECTORSIZE]);

int GetPosition(int* row, int* col, Color Board[][MATRIXSIZE], Player CurrentPlayer, Vector vector[]);

int CheckValidInput(int row, int col, Color Board[][MATRIXSIZE]);

int CheckIfCanDoAnyBridge(Player CurrentPlayer, Color Board[MATRIXSIZE][MATRIXSIZE], int row, int col, Vector vector[]);

int CheckBridgeInDirection(Player CurrentPlayer, Color Board[MATRIXSIZE][MATRIXSIZE], int x, int y, Direction direction, Vector vector[]);

void PrintBoard(Player Board[][MATRIXSIZE]);

int CurrenRecord(Player board[][MATRIXSIZE], Player player);

