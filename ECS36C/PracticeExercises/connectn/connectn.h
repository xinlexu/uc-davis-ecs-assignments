#ifndef CONNECTN_H
#define CONNECTN_H

int IsValidInput();

void CreateBoard(int board[ ][505], int num_rows, int num_columns);

int GameOver(int board[ ][505], int num_rows, int num_columns, int num_pieces);

void ChangeBoard(int board[ ][505], int position, int player);

int IsValidPos(int board[ ][505], int num_rows, int num_columns, int position);

#endif
