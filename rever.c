#include "rever.h"
#include <stdio.h>

int MainGame()
{
	Player CurrentPlayer = X;
	Color Board[MATRIXSIZE][MATRIXSIZE];
	Vector vector[8];
	int row, col;

	InitBoard(Board);
	InitVector(vector);

	while (1)
	{
		if (DoesHaveATurn(Board, X, vector) > 0 || DoesHaveATurn(Board, O, vector) > 0)
		{
			ClearAllPossibleMoves(Board);
			if (!BoardIsFull(Board) && DoesHaveATurn(Board, CurrentPlayer, vector) > 0)
			{

				PrintBoard(Board);
				for (int i = 0; i < 3; i++)
				{
					if(CurrentPlayer==X)
						printf("\tIts player  -- X -- turn\n");
					else if (CurrentPlayer == O)
							printf("\tIts player  -- O -- turn\n");
					if (GetPosition(&row, &col, Board, CurrentPlayer, vector) == 1) {
						break;
					}

				}


			}
			else if (!BoardIsFull(Board))
			{
				int winner = SearchForWinner(Board);
				printf("The Winner is: %d", winner);
				break;
			}
			//posibilty of adding a clue to the user after not succeeding 3 times
			ChangePlayer(&CurrentPlayer);
		}
		else
		{
			int winner = SearchForWinner(Board);
			printf("The Winner is: %d", winner);
			break;
		}

	}



}

void ClearAllPossibleMoves(Color Board[][MATRIXSIZE])
{
	for (int i = 1; i <= GAMESIZE; i++)
	{
		for (int j = 1; j <= GAMESIZE; j++)
		{
			if (Board[i][j] == POSIBBLEMOVE)
				Board[i][j] = EMPTY;

		}
	}
}

void ChangePlayer(Player* player)
{
	switch (*player)
	{
	case(X):
		*player = O;
		break;
	case(O):
		*player = X;
		break;

	}
}


void InitBoard(Color Board[][MATRIXSIZE])
{
	for (int i = 0; i < MATRIXSIZE; i++)
	{
		for (int j = 0; j < MATRIXSIZE; j++)
		{
			Board[i][j] = EMPTY;
		}
	}
	Board[4][5] = X;
	Board[4][4] = O;
	Board[5][5] = O;
	Board[5][4] = X;
}


void InitVector(Vector vector[VECTORSIZE])
{
	vector[UP].row = -1;
	vector[UP].col = 0;
	vector[UPRIGHT].row = -1;
	vector[UPRIGHT].col = 1;
	vector[RIGHT].row = 0;
	vector[RIGHT].col = 1;
	vector[DOWNRIGHT].row = 1;
	vector[DOWNRIGHT].col = 1;
	vector[DOWN].row = 1;
	vector[DOWN].col = 0;
	vector[DOWNLEFT].row = 1;
	vector[DOWNLEFT].col = -1;
	vector[LEFT].row = 0;
	vector[LEFT].col = -1;
	vector[UPLEFT].row = -1;
	vector[UPLEFT].col = -1;
}


//Gets the input of the user
int GetPosition(int* row, int* col, Color Board[][MATRIXSIZE], Player CurrentPlayer, Vector vector[])
{
	printf("\nInsert row and col...  i.e. 2 3\n");
	scanf_s("%d %d", row,col);

	if (CheckValidInput(*row, *col, Board) && CheckIfCanDoAnyBridge(CurrentPlayer, Board, *row, *col, vector)) {
		Board[*row][*col] = CurrentPlayer;
		return 1;
	}
	printf("Wrong input\n");
	return 0;


}

int DoesHaveATurn(Color Board[][MATRIXSIZE], Player CurrentPlayer, Vector vector[])
{
	int counter = 0;
	for (int i = 1; i <= GAMESIZE; i++)
	{
		for (int j = 1; j <= GAMESIZE; j++)
		{
			if (Board[i][j] != EMPTY && Board[i][j] != CurrentPlayer && Board[i][j] != POSIBBLEMOVE) {
				counter += SetPossibleMoves(Board, CurrentPlayer, i, j, vector);
			}

		}
	}
	return counter;
}

int CheckValidInput(int row, int col, Color Board[][MATRIXSIZE])
{
	if (row < 1 || row > GAMESIZE || col < 1 || col > GAMESIZE)
	{
		return 0;
	}
	if (Board[row][col] != EMPTY && Board[row][col] != POSIBBLEMOVE)
	{
		return 0;
	}

	return 1;
}

int CheckIfCanDoAnyBridge(Player CurrentPlayer, Color Board[MATRIXSIZE][MATRIXSIZE], int row, int col, Vector vector[]) {

	int CanDoBridge = 0;
	if (CheckValidInput(row, col, Board))
	{
		if (CheckBridgeInDirection(CurrentPlayer, Board, col, row, UP, vector) > 1)CanDoBridge++;
		if (CheckBridgeInDirection(CurrentPlayer, Board, col, row, UPRIGHT, vector) > 1)CanDoBridge++;
		if (CheckBridgeInDirection(CurrentPlayer, Board, col, row, RIGHT, vector) > 1)CanDoBridge++;
		if (CheckBridgeInDirection(CurrentPlayer, Board, col, row, DOWNRIGHT, vector) > 1)CanDoBridge++;
		if (CheckBridgeInDirection(CurrentPlayer, Board, col, row, DOWN, vector) > 1)CanDoBridge++;
		if (CheckBridgeInDirection(CurrentPlayer, Board, col, row, DOWNLEFT, vector) > 1)CanDoBridge++;
		if (CheckBridgeInDirection(CurrentPlayer, Board, col, row, LEFT, vector) > 1)CanDoBridge++;
		if (CheckBridgeInDirection(CurrentPlayer, Board, col, row, UPLEFT, vector) > 1)CanDoBridge++;
	}
	return CanDoBridge;
}

int CheckBridgeInDirection(Player CurrentPlayer, Color Board[MATRIXSIZE][MATRIXSIZE], int x, int y, Direction direction, Vector vector[]) {
	int IsBridge = 0;
	//moving the position
	x += vector[direction].col;
	y += vector[direction].row;
	if (Board[y][x] == EMPTY || Board[y][x] == POSIBBLEMOVE) {
		return 0;
	}
	if (Board[y][x] == CurrentPlayer) {
		return 1;
	}
	Board[y][x] = CurrentPlayer;
	IsBridge = CheckBridgeInDirection(CurrentPlayer, Board, x, y, direction, vector) == 0 ? 0 : 2;
	if (!IsBridge) {
		Board[y][x] = Board[y][x] == X ? O : X;
	}
	return IsBridge;
}


int SetPossibleMoves(Color Board[][MATRIXSIZE], Player CurrentPlayer, int row, int col, Vector vector[])
{

	int counter = 0;
	int j, i;

	//runs over the rows around the corrent position
	for (i = row - 1; i < row + 2; i++)
	{
		//in case the row in out of bounds,dont keep on checking the position
		if (i >= 1 && i <= GAMESIZE) {
			//runs over the column around the corrent position
			for (j = col - 1; j < col + 2; j++)
			{
				//in case the column in out of bounds,dont keep on checking the position
				if (j >= 1 && j <= GAMESIZE)
				{
					if (Board[i][j] == EMPTY) {
						if (!(i == row && j == col)) {
							if (CheckIfCanDoAnyBridge2(CurrentPlayer, Board, i, j, vector) > 0)
							{
								counter++;
								Board[i][j] = POSIBBLEMOVE;
							}

						}
					}
				}
			}
		}
	}
	return counter;

}

int CheckIfCanDoAnyBridge2(Player CurrentPlayer, Color Board[MATRIXSIZE][MATRIXSIZE], int row, int col, Vector vector[]) {

	int CanDoBridge = 0;
	if (CheckValidInput(row, col, Board))
	{
		if (CheckBridgeInDirection2(CurrentPlayer, Board, col, row, UP, vector) > 1)CanDoBridge++;
		if (CheckBridgeInDirection2(CurrentPlayer, Board, col, row, UPRIGHT, vector) > 1)CanDoBridge++;
		if (CheckBridgeInDirection2(CurrentPlayer, Board, col, row, RIGHT, vector) > 1)CanDoBridge++;
		if (CheckBridgeInDirection2(CurrentPlayer, Board, col, row, DOWNRIGHT, vector) > 1)CanDoBridge++;
		if (CheckBridgeInDirection2(CurrentPlayer, Board, col, row, DOWN, vector) > 1)CanDoBridge++;
		if (CheckBridgeInDirection2(CurrentPlayer, Board, col, row, DOWNLEFT, vector) > 1)CanDoBridge++;
		if (CheckBridgeInDirection2(CurrentPlayer, Board, col, row, LEFT, vector) > 1)CanDoBridge++;
		if (CheckBridgeInDirection2(CurrentPlayer, Board, col, row, UPLEFT, vector) > 1)CanDoBridge++;
	}
	return CanDoBridge;
}



int CheckBridgeInDirection2(Player CurrentPlayer, Color Board[MATRIXSIZE][MATRIXSIZE], int x, int y, Direction direction, Vector vector[]) {
	int IsBridge = 0;
	//moving the position
	x += vector[direction].col;
	y += vector[direction].row;
	if (Board[y][x] == EMPTY || Board[y][x] == POSIBBLEMOVE) {
		return 0;
	}
	if (Board[y][x] == CurrentPlayer) {
		return 1;
	}
	IsBridge = CheckBridgeInDirection2(CurrentPlayer, Board, x, y, direction, vector) == 0 ? 0 : 2;
	return IsBridge;
}


int BoardIsFull(Color Board[][MATRIXSIZE])
{
	for (int i = 1; i <= GAMESIZE; i++)
	{
		for (int j = 1; j <= GAMESIZE; j++)
		{
			if (Board[i][j] == EMPTY) return 0;
		}
	}
	return 1;
}

Player SearchForWinner(Color Board[][MATRIXSIZE])
{
	int X = 0, O = 0;
	for (int i = 1; i <= GAMESIZE; i++)
	{
		for (int j = 1; j <= GAMESIZE; j++)
		{
			if (Board[i][j] == X)
				X++;
			else if (Board[i][j] == O)
				O++;
		}
	}
	return X > O ? X : O;
}
//prints the Board of the game
void PrintBoard(Player board[][MATRIXSIZE])
{
	system("cls");

	int o, x;
	for (int i = 1; i <= 8; i++)
		{
			printf("   %d    ", i);
		}
		printf("\n");
	for (int i = 1; i < MATRIXSIZE - 1; i++)
	{
		

		printf("   _____  _____  _____  _____  _____  _____  _____  _____\n");
		printf("%d |", i);
		for (int j = 1; j < MATRIXSIZE - 1; j++) {
			if (board[i][j] == O)
				printf("   X  |");
			else if (board[i][j] == X)
				printf("   O  |");
			else if(board[i][j]==POSIBBLEMOVE)
				printf("   *  |");
				else if (board[i][j] == EMPTY)  printf("      |");
		}
		printf("\n");
	}
	printf("   _____  _____  _____  _____  _____  _____  _____  _____\n");
	o = CurrenRecord(board, O);
	x = CurrenRecord(board, X);
	printf("\n");
	printf("\n\t\tX -> %d\t\tO -> %d\n\n\n", o, x);
}

int CurrenRecord(Player board[][MATRIXSIZE], Player player)
{
	int count = 0;
	for (int i = 0; i < GAMESIZE; i++)
		for (int j = 0; j < GAMESIZE; j++)
			if (board[i][j] == player)
				count++;
	return count;
}
/*
void PrintBoard(Color Board[MATRIXSIZE][MATRIXSIZE])
{

	int O, yellow;
	for (int i = 1; i < MATRIXSIZE - 1; i++)
	{

		printf("  _____  _____  _____  _____  _____  _____  _____  _____\n|");
		for (int j = 1; j < MATRIXSIZE - 1; j++) {
			if (Board[i][j] == O)
				printf("   X  |");
			else if (Board[i][j] == X)
				printf("   O  |");
			else if (Board[i][j] == EMPTY)  printf("      |");
		}
		printf("\n");
	}
	O = currentRecord(Board, O);
	yellow = currentRecord(Board, X);
	printf("\n");
	printf("\n\t\tX : %d\t\tO : %d\n\n", O, yellow);
/*int i = 0, j = 0;
	printf(" |");
	for (i = 0; i < GAMESIZE; i++) {
		printf("-%d-|", i + 1);
	}
	putchar('\n');
	for (i = 1; i <= GAMESIZE; i++)
	{
		printf("%d", i);
		for (j = 1; j <= GAMESIZE; j++)
		{
			printf("   ");

			switch (Board[i][j])
			{
			case(X):
				printf(XCOLOR "*" RESET);
				//printf("%d", Board[i][j]);
				break;
			case(O):
				printf(OCOLOR "*" RESET);
				//printf("%d", Board[i][j]);
				break;
			case(POSIBBLEMOVE):
				printf("*");
				break;
			default:
				printf(" ");
				break;
			}



		}
		putchar('\n');
		printf(" |");
		for (j = 1; j <= GAMESIZE; j++)
		{

			printf("---|");
		}
		putchar('\n');
	}

	
}*/
