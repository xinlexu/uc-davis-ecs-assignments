#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int IsValidInput() {
	int IsValidInput = 1;
	char letter;
	do {
		scanf("%c", &letter);
		if (!isspace(letter)) {
			IsValidInput = 0;
		}
	} while (letter != '\n');
	return IsValidInput;
}

void CreateBoard(int board[ ][505], int num_rows, int num_columns) {
	int i, j;
	int isTwoDigits = 0;
	if (num_rows > 10) {
		isTwoDigits = 1;
	}
	for (i = num_rows - 1; i >= 0; i--) {
		for (j = -1; j < num_columns; j++) {
			if (j == -1) {
				if (isTwoDigits && i < 10) {
					printf (" ");
				}
				printf("%d ", i);
			}
			else if (board[i][j] == 0) {
				printf("* ");
			}
			else if (board[i][j] == 1) {
				printf("X ");
			}
			else {
				printf("O ");
			}
		}
		printf("\n");
	}
	if (isTwoDigits) {
		printf (" ");
	}
	printf("  ");
	for (i = 0; i < num_columns; i++) {
		printf("%d ", i);
	} 
	printf("\n");
}

int GameOver(int board[ ][505], int num_rows, int num_columns, int num_pieces) {
	int i, j, k;
	int increase = num_pieces - 1;
	for (i = 0; i < num_rows; i++) {
		for (j = 0; j < num_columns; j++) {
			if (board[i][j] == 0)
				continue; 
			if (i + increase < num_rows) {
				for (k = 1; k <= increase; k++) {
					if (board[i+k][j] != board[i+k-1][j])
						break;
				}
				if (k > increase) {
					return board[i][j];
				}
			}
			if (j + increase < num_columns) {
				for (k = 1; k <= increase; k++) {
					if (board[i][j+k] != board[i][j+k-1])
						break;
				}
				if (k > increase) {
					return board[i][j];
				}
			}
			if (i + increase < num_rows && j + increase < num_columns)
			{
				for (k = 1; k <= increase; k++) {
					if (board[i+k][j+k] != board[i+k-1][j+k-1])
						break;
				}
				if (k > increase) {
					return board[i][j];
				} 				
			}
			if (i - increase >= 0 && j + increase < num_columns)
			{
				for (k = 1; k <= increase; k++) {
					if (board[i-k][j+k] != board[i-k+1][j+k-1])
						break;
				}
				if (k > increase) {
					return board[i][j];
				}
			}
		}
	}

	for (i = 0; i < num_rows; i++) {
		for (j = 0; j < num_columns; j++) {
			if (board[i][j] == 0)
				return 0;
		}
	}
	return 3;
}

void ChangeBoard(int board[ ][505], int position, int player) {
	int i;
	for (i = 0; 1; i++) {
		if (board[i][position] == 0) {
			board[i][position] = player;
			return ;
		}
	}
}

int IsValidPos(int board[ ][505], int num_rows, int num_columns, int position)  {
	if (position < 0 || position >= num_columns) {
		return 0;
	}
	if (board[num_rows - 1][position] != 0) {
		return 0;
	}
	return 1;
}
