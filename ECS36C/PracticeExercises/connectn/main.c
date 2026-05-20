#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "connectn.h"

int main(int argc, char *argv[]) {	
    if (argc < 4) {
        printf("Not enough arguments entered\n");
        printf("Usage connectn.out num_rows num_columns\nnumber_of_pieces_in_a_row_needed_to_win");
        exit(0);
    } 
    else if (argc > 4) {
        printf("Too many arguments entered\n");
        printf("Usage connectn.out num_rows num_columns\nnumber_of_pieces_in_a_row_needed_to_win");
        exit(0);
    }
    
    int num_rows, num_columns, num_pieces, flag = 0, position, player = 1;
    int board[505][505] = {};

    num_rows = atoi(argv[1]);
    num_columns = atoi(argv[2]);
    num_pieces = atoi(argv[3]); 
    CreateBoard(board, num_rows, num_columns);

    while (flag == 0) {
        do {
        	printf("Enter a column between 0 and %d to play in: ", num_columns - 1); 
            scanf("%d", &position);
        }
        while (!IsValidPos(board, num_rows, num_columns, position));
        ChangeBoard(board, position, player);
        player = 2 / player;
        CreateBoard(board, num_rows, num_columns);
        flag = GameOver(board, num_rows, num_columns, num_pieces);
    }

    if (flag == 1) {
        printf("Player 1 Won!\n");
    }
    else if (flag == 2) {
        printf("Player 2 Won!\n");
    }
    else {
        printf("Tie game!\n");
    }


    return 0;
}
