#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "board.h"
#include "cpu.h"

int SIZE = 8;
int E = 0;
int b = 0b001;
int B = 0b101;
int w = 0b010;
int W = 0b110;
int IS_KING = 0b100;

/*
int board[64] = {
	1, 0, 1, 0, 1, 0, 1, 0,
	0, 1, 0, 1, 0, 1, 0, 1,
	1, 0, 1, 0, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 4, 0, 12, 0,
	0, 4, 0, 0, 0, 4, 0, 0,
	4, 0, 4, 0, 4, 0, 0, 0,
	0, 0, 0, 4, 0, 0, 0, 4,
};*/
/*
int board[64] = {
	1, 0, 1, 0, 1, 0, 1, 0,
	0, 1, 0, 1, 0, 1, 0, 1,
	1, 0, 1, 0, 1, 0, 1, 0,
	0, 4, 0, 0, 0, 4, 0, 4,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 4, 0, 4, 0, 4, 0, 4,
	4, 0, 4, 0, 0, 0, 4, 0,
	0, 4, 0, 0, 0, 4, 0, 4,
};*/
/*int board[64] = {
	1, 0, 1, 0, 1, 0, 1, 0,
	0, 1, 0, 1, 0, 1, 0, 1,
	1, 0, 1, 0, 1, 0, 1, 0,
	0, 4, 0, 4, 0, 4, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 4, 0, 4, 0, 4, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};*/


int test(int* x){
	printf("  %i @ %p\n", *x, x);
	*x = *x + 1;
	printf("  %i @ %p\n", *x, x);
}

int main()
{
	puts("Checkers");
    srand(time(NULL));

   /* int board[64] = {
        b, 0, 0, 0, W ,0, b, 0,
        0, 0, 0, 0, 0, 0, 0, b,
        b, 0, b, 0, 0, 0, 0, 0,
        0, b, 0, 0, 0, 0, 0, w,
        w, 0, w, 0, 0, 0, w, 0,
        0, 0, 0, w, 0, 0, 0, w,
        w, 0, w, 0, B, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };*/

	int board[64] = {
        0, 0, 0, 0, W ,0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        b, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, b, 0, W, 0, w,
        b, 0, 0, 0, 0, 0, b, 0,
        0, 0, 0, w, 0, 0, 0, b,
        w, 0, w, 0, 0, 0, 0, 0,
        0, 0, 0, w, 0, w, 0, w,
    };
    //calcEndangered(board, w);

	Move moves[12 * 4];  // 12 pieces, 4 moves per piece
	printBoard(board);

	while(1){
		char cmd[100];
		int i1; int i2; int i3; int i4;
		scanf ("%s %i %i %i %i", cmd, &i1, &i2, &i3, &i4);

		if(strcmp(cmd, "move") == 0){
			Move m = {.y = i1, .x = i2, ._y = i3, ._x = i4 };
			Move _moves[4];
			makeMove(board, m, _moves);
			printBoard(board);
		}else

		if(strcmp(cmd, "get") == 0){
			int nmoves = getMoves(board, moves, i1);
			printf("\n%i valid moves found\n", nmoves);
            for(int z = 0; z < nmoves; z++){
				printf("  %i %i -> %i %i\n", moves[z].y, moves[z].x, moves[z]._y, moves[z]._x);
            }
		}else

		if(strcmp(cmd, "print") == 0){
            printBoard(board);
		}else

		if(strcmp(cmd, "set") == 0){
            board[i1 * SIZE + i2] = i3;
            printBoard(board);
		}else
		if(strcmp(cmd, "reset") == 0){
            resetBoard(board);
            printBoard(board);
		}else
		if(strcmp(cmd, "c") == 0){
			cpu_move(board, i1);
			printBoard(board);
		}else
		{
			// Unknown command
		}
	}

    return 0;
}










































