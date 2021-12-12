#include <unistd.h>	   // open() close()
#include <fcntl.h>	   // O_* constants
#include <sys/stat.h>  // mkfifo()
#include <sys/types.h> // mkfifo()
#include <string.h>	   // strlen() strcmp()
#include <stdio.h>	   // printf() scanf()
#include <stdlib.h>	   // exit()
#include <iostream>

#include "my_const.h"
using namespace std;

void populateBoard(char[3][3]);
void checkBoard(char[3][3], char);
void p2checkBoard(char[3][3], char);
void printBoard(char[3][3]);
bool winConditionCheckP1(char[3][3]);
bool winConditionCheckP2(char[3][3]);
bool validate(char[3][3], char ar);

int main()
{

	// create the named pipe (FIFO) if not exist
	int f1 = mkfifo(myfifo_1to2, 0666);
	int f2 = mkfifo(myfifo_2to1, 0666);
	char tictac[3][3]; //tic tac toe
	int counter = 1;
	char p1;
	char p2;
	bool isValid;

	populateBoard(tictac); //sets up tic tac

	char rd_data[MAX], wr_data[MAX];
	printf("Welcome to TIC TAC TOE!!!!! ... \n");
	printf("Waiting for player 2 to join ... \n");

	// P1&P2: order of open() is important to unblock processes
	// open() for WR will be blocked until the other side is open for RD
	int fd_wr = open(myfifo_1to2, O_WRONLY);
	// open() for RD will be blocked until the other side is open for WR
	int fd_rd = open(myfifo_2to1, O_RDONLY);

	printf("Player 2 has joined\n");

	printBoard(tictac);

	// prog1: write first
	while (true)
	{
		if (winConditionCheckP1(tictac)) //check if p1 wins
		{
			printf("PLAYER 1 IS VICTORIOUS\n");
			break;
		}
		if (winConditionCheckP2(tictac)) //check if p2 wins
		{
			printf("PLAYER 2 IS VICTORIOUS\n");
			break;
		}

		printf("Enter a number (Q to quit): \n");
		cin >> p1;
		wr_data[0] = p1;

		if (p1 == 'Q')
		{ //quit condition
			write(fd_wr, wr_data, strlen(wr_data) + 1);

			printf("YOU QUIT! YOU LOSE!\n");
			break;
		}

		isValid = validate(tictac, p1);

		if (isValid)
		{
			checkBoard(tictac, p1); // function call to input move and update board
			printBoard(tictac);
			if (winConditionCheckP1(tictac)) //check if p1 wins
			{
				printf("PLAYER 1 IS VICTORIOUS\n");
				write(fd_wr, wr_data, strlen(wr_data) + 1);
				break;
			}

			printf("Waiting for opponent.....\n");

			write(fd_wr, wr_data, strlen(wr_data) + 1);

			read(fd_rd, rd_data, sizeof(rd_data));

			if (strcmp(rd_data, "Q") == 0)
			{
				printf("OPPONENT HAS QUIT! YOU WIN!\n");
				break;
			}

			p2 = rd_data[0];
			p2checkBoard(tictac, p2);		 //input  p2s move
			printBoard(tictac);				 //print upon receiven
			if (winConditionCheckP2(tictac)) //check if p2 wins
			{
				printf("PLAYER 2 IS VICTORIOUS\n");
				break;
			}
		}
		else
		{
			printBoard(tictac);
			printf("Invalid input, try again\n");
			continue;
		}
	}
	close(fd_wr);
	close(fd_rd);
	unlink(myfifo_1to2);
	unlink(myfifo_2to1);
	printf("GAME OVER\n");
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

void p2checkBoard(char tictac[3][3], char ar)
{
	for (int i = 0; i < 3; i++)
	{ //search 2d array
		for (int j = 0; j < 3; j++)
		{
			if (tictac[i][j] == ar)
			{
				tictac[i][j] = 'O';
			}
		}
	}
}

bool winConditionCheckP1(char tictac[3][3])
{
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
{
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
