#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "my_const.h"
using namespace std;

void populateBoard(char[3][3]);
void checkBoard(char[3][3], char);
void p2checkBoard(char[3][3], char);
void printBoard(char[3][3]);
bool validate(char[3][3], char);
bool winConditionCheckP1(char[3][3]);
bool winConditionCheckP2(char[3][3]);

int main()
{
	// create the named pipe (FIFO) if not created yet
	int f1 = mkfifo(myfifo_1to2, 0666);
	int f2 = mkfifo(myfifo_2to1, 0666);
	char tictac[3][3];
	char p1;
	char p2;
	bool isValid;

	populateBoard(tictac);

	char rd_data[MAX], wr_data[MAX];
	printf("Welcome to TIC TAC TOE!!!!! ... \n");
	printf("Waiting for player 1 to join ... \n");

	// P1&P2: order of open is important to unblock process
	// open() for RD will be blocked until the other side is open for WR
	int fd_rd = open(myfifo_1to2, O_RDONLY);
	// open() for WR will be blocked until the other side is open for RD
	int fd_wr = open(myfifo_2to1, O_WRONLY);

	printf("Player 1 has joined \n");

	printBoard(tictac);

	// prog2: read first
	while (true)
	{
		if (winConditionCheckP2(tictac)) //check if p2 wins
		{
			printf("PLAYER 2 IS VICTORIOUS\n"); /////why is this looping?
			break;
		}
		if (p2 == 'Q')
		{ //upon entering Q in inner loop this is how we will break outer loop and quit game
			printf("YOU QUIT! YOU LOSE!\n");
			break;
		}
		read(fd_rd, rd_data, sizeof(rd_data));

		if (strcmp(rd_data, "Q") == 0)
		{ //quit condition from p1
			printf("OPPONENT HAS QUIT! YOU WIN!\n");
			break;
		}
		// rd_data[strlen(rd_data) - 1] = '\0';

		p1 = rd_data[0];
		checkBoard(tictac, p1);			 //checking players 1 move
		printBoard(tictac);				 // print upon receive
		if (winConditionCheckP1(tictac)) //check if p1 wins
		{
			printf("PLAYER 1 IS VICTORIOUS\n");
			break;
		}

		//model after prog1 below
		while (true)
		{

			printf("Enter a number (Q to quit): \n");
			cin >> p2;

			wr_data[0] = p2;

			if (p2 == 'Q')
			{
				write(fd_wr, wr_data, strlen(wr_data) + 1);
				printf("YOU QUIT! YOU LOSE!\n");
				break;
			}

			isValid = validate(tictac, p2);

			if (isValid)
			{
				p2checkBoard(tictac, p2);
				if (winConditionCheckP2(tictac)) //check if p2 wins
				{
					write(fd_wr, wr_data, strlen(wr_data) + 1);
					break;
				}

				write(fd_wr, wr_data, strlen(wr_data) + 1);

				printBoard(tictac); //print after send
				break;
			}
			else
			{
				printBoard(tictac);
				printf("Invalid input, try again\n");
				continue;
			}
		}
	}
	close(fd_rd);
	close(fd_wr);
	printf("GAME OVER\n");
}

void printBoard(char tictac[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("|%c", tictac[i][j]);
		}
		printf("|\n");
		printf("---------\n");
	}
}

void checkBoard(char tictac[3][3], char ar)
{
	for (int i = 0; i < 3; i++)
	{ //search 2d array
		for (int j = 0; j < 3; j++)
		{
			if (tictac[i][j] == ar)
			{ //if p1 choice is equal to num on board, change to X
				tictac[i][j] = 'X';
			}
		}
	}
}

void populateBoard(char tictac[3][3])
{

	tictac[0][0] = '1';
	tictac[0][1] = '2';
	tictac[0][2] = '3';
	tictac[1][0] = '4';
	tictac[1][1] = '5';
	tictac[1][2] = '6';
	tictac[2][0] = '7';
	tictac[2][1] = '8';
	tictac[2][2] = '9';
}

void p2checkBoard(char tictac[3][3], char ar)
{
	for (int i = 0; i < 3; i++)
	{ //search 2d array
		for (int j = 0; j < 3; j++)
		{
			if (tictac[i][j] == ar)
			{ //if p1 choice is equal to num on board, change to X
				tictac[i][j] = 'O';
			}
		}
	}
}

bool validate(char tictac[3][3], char ar)
{

	for (int i = 0; i < 3; i++)
	{ //search 2d array
		for (int j = 0; j < 3; j++)
		{
			if (tictac[i][j] == ar && ar != 'X' && ar != 'O')
			{ //if p1 choice is equal to num on board, is valid
				return true;
			}
		}
	}
	return false;
}

bool winConditionCheckP1(char tictac[3][3])
{																		   //come back to this
	if (tictac[0][0] == 'X' && tictac[0][1] == 'X' && tictac[0][2] == 'X') //horizontal
		return true;
	else if (tictac[1][0] == 'X' && tictac[1][1] == 'X' && tictac[1][2] == 'X')
		return true;
	else if (tictac[2][0] == 'X' && tictac[2][1] == 'X' && tictac[2][2] == 'X')
		return true;

	else if (tictac[0][0] == 'X' && tictac[1][0] == 'X' && tictac[2][0] == 'X') //vertical
		return true;
	else if (tictac[0][1] == 'X' && tictac[1][1] == 'X' && tictac[2][1] == 'X')
		return true;
	else if (tictac[0][2] == 'X' && tictac[1][2] == 'X' && tictac[2][2] == 'X')
		return true;

	else if (tictac[0][0] == 'X' && tictac[1][1] == 'X' && tictac[2][2] == 'X') //diagonal
		return true;
	else if (tictac[2][0] == 'X' && tictac[1][1] == 'X' && tictac[0][2] == 'X')
		return true;

	else
		return false;
}
bool winConditionCheckP2(char tictac[3][3])
{																		   //come back to this
	if (tictac[0][0] == 'O' && tictac[0][1] == 'O' && tictac[0][2] == 'O') //horizontal
		return true;
	else if (tictac[1][0] == 'O' && tictac[1][1] == 'O' && tictac[1][2] == 'O')
		return true;
	else if (tictac[2][0] == 'O' && tictac[2][1] == 'O' && tictac[2][2] == 'O')
		return true;

	else if (tictac[0][0] == 'O' && tictac[1][0] == 'O' && tictac[2][0] == 'O') //vertical
		return true;
	else if (tictac[0][1] == 'O' && tictac[1][1] == 'O' && tictac[2][1] == 'O')
		return true;
	else if (tictac[0][2] == 'O' && tictac[1][2] == 'O' && tictac[2][2] == 'O')
		return true;

	else if (tictac[0][0] == 'O' && tictac[1][1] == 'O' && tictac[2][2] == 'O') //diagonal
		return true;
	else if (tictac[2][0] == 'O' && tictac[1][1] == 'O' && tictac[0][2] == 'O')
		return true;

	else
		return false;
}