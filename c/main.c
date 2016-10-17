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
int IS_PIECE = 0b011;

int log = 0;
int logv = 0;
int logvv = 0;

int main()
{
	puts("Checkers");
    srand(time(NULL));

	int board[64] = {
        b, 0, b, 0, b ,0, 0, 0,
        0, b, 0, b, 0, b, 0, b,
        b, 0, b, 0, 0, 0, b, 0,
        0, 0, 0, 0, 0, b, 0, 0,
        w, 0, 0, 0, w, 0, 0, 0,
        0, 0, 0, w, 0, b, 0, w,
        w, 0, w, 0, w, 0, w, 0,
        0, w, 0, w, 0, 0, 0, w,
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
		if(strcmp(cmd, "run") == 0){
			run(10);
		}else
		{
			// Unknown command
		}
	}

    return 0;
}

void run(int nruns){
    int n; int c; int z;

    int board[SIZE*SIZE];
    int _board[SIZE*SIZE];
    resetBoard(board);

    for(n = 0; n < 1; n++){
        int nmoves = 1;
        c = 0;
        while(c < 30){
            c++;
            nmoves = cpu_move(board, b);
            if(nmoves == 0) break;

            for(z = 0; z< 64; z++) _board[z] = board[z];

            nmoves = cpu_move(board, w);
            if(nmoves == 0) break;

            board_print2(_board, board);
        }
    }
    printf("\nFinal score | b %i | w %i\n", cpu_calcScore(board, b, 4), cpu_calcScore(board, w, 4));

    board_print2(board, board);

}








































