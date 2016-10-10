#include "main.h"
#include "board.h"
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

char pStr[] = "_bw3KBW7";
int validHops [4][2] = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};
int validHits [4][2] = {{2, 2}, {2, -2}, {-2, -2}, {-2, 2}};

int makeMove(int board[], Move m, Move moves[]){
    int piece = board[m.y*SIZE+m.x];

    board[m.y *SIZE+m.x ] = E;
    board[m._y*SIZE+m._x] = piece;

	// Remove piece that was hit
    if(abs(m.y - m._y) == 2)
		board[(m.y+m._y)/2*SIZE + (m.x+m._x) / 2] = E;
    // Promote black piece
    if(m._y == SIZE - 1 && piece & b)
		board[m._y * SIZE + m._x] = piece | IS_KING;
	// Promote white piece
	if(m._y == 0 && piece & w && !(piece & IS_KING))
		board[m._y * SIZE + m._x] = piece | IS_KING;

	// See if there is a next move that has to be taken after a hit
	int z; int counter = 0;
	if(abs(m.y - m._y) == 2){
		for(z = 0; z < 4; z++){
			int _y = m._y + validHits[z][0];
			int _x = m._x + validHits[z][1];
			Move move = {.y = m._y, .x = m._x, ._y = _y, ._x = _x};
			if(isMove(board, move)){
				moves[counter] = move;
				counter++;
			}
		}
	}
	//if(counter > 0){
        //printf("move : found %i follow-up moves\n", counter);
        //for(z = 0; z < counter; z++){
            // printf("\t%i : %i %i -> %i %i\n", z, moves[z].y, moves[z].x, moves[z]._y, moves[z]._x);
        //}
	//}
	return counter;
}

void printBoard(int board[]){

    int x; int y;
    // First row
    printf("\n  ");
    for(x = 0; x < SIZE; x++){
        printf("%i ", x);
    }
    printf("\n");
	// Middle rows
    for(y = 0; y < SIZE; y++){
        printf("%i|", y);
        for(x = 0; x < SIZE; x++){
            printf("%c|", pStr[board[y * SIZE + x]]);
        }
        printf("\n");
    }

    return;
}

void resetBoard(int board[]){
    int x; int y;
    for(y = 0; y < SIZE; y++){
        for(x = 0; x < SIZE; x++){
            int p = 0;
            if((y+x) % 2 == 0){
                p = y < 3 ? b : p;
                p = y > 4 ? w : p;
            }
            board[y * SIZE + x] = p;
        }
    }
}

int getMoves(int board[], Move moves[], int piece){
	// printf("\nGetting all valid moves for piece '%c'\n", pStr[piece]);
	int counter = 0;

	int x; int y; int z;
    // Find all hits
    for(y = 0; y < SIZE; y++){
        for(x = 0; x < SIZE; x++){
            int p = board[y * SIZE + x];
            if(!(p & piece)){
				continue;
            }

            for(z = 0; z < 4; z++){
                int _y = y + validHits[z][0];
                int _x = x + validHits[z][1];
                Move move = {.y = y, .x = x, ._y = _y, ._x = _x};
                if(isMove(board, move)){
					moves[counter] = move;
					counter++;
                }

            }
        }
    }
	// If there are no hits, find all moves
	if(counter == 0){
		for(y = 0; y < SIZE; y++){
			for(x = 0; x < SIZE; x++){
				int p = board[y * SIZE + x];
				if(!(p & piece)){
					continue;
				}

				for(z = 0; z < 4; z++){
					int _y = y + validHops[z][0];
					int _x = x + validHops[z][1];
					Move move = {.y = y, .x = x, ._y = _y, ._x = _x};
					if(isMove(board, move)){
						moves[counter] = move;
						counter++;
					}
				}
			}
		}
	}

    return counter;
}

int isMove(int board[], Move move){

    //printf("  isMove %c %i %i -> %i %i   ", pStr[board[move.y*SIZE + move.x]], move.y, move.x, move._y, move._x);
	// Check y and _y in board
	if(move.y < 0  || SIZE - 1 < move.y  || move.x < 0  || SIZE - 1 < move.x){
	//	printf("\t'from' coord out of range\n");
		return 0;
	}
	// Check x and _x in board
	if(move._y < 0 || SIZE - 1 < move._y || move._x < 0 || SIZE - 1 < move._x){
	//	printf("\t'to' coord out of range\n");
		return 0;
	}
	// Check empty
	if(board[move._y * SIZE + move._x] != E){
	//	printf("\t'to' not empty\n");
		return 0;
	}

	int piece = board[move.y * SIZE + move.x];
	int dir   = piece & b ? 1 : -1;
	int other = ~piece & (b | w);

	int dy = abs(move.y - move._y);
	int dx = abs(move.x - move._x);

	// Check y-distance
	if(dy != 1 && dy != 2){
	//	printf("\ty-distance incorrect\n");
		return 0;
	}
	// Check x-distance
	if(dx != 1 && dx != 2){
	//	printf("\tx-distance incorrect\n");
		return 0;
	}
	// Check y-distance == x-distance
	if(dy != dx){
	//	printf("\ty-distance not equal to x-distance\n");
		return 0;
	}
	// Check hit
	if(dy == 2){
		int hit = board[((move.y+move._y)/2)*SIZE + (move.x+move._x)/2];
		if(!(hit & other)){
	//		printf("\tno piece hit\n");
			return 0;
		}
	}
	// Check direction for normal piece
    if(!(piece & IS_KING || abs(move._y - move.y - dir) < 2)){
	//	printf("\tpiece goes in wrong direction\n");
		return 0;
    }

	//printf("\n");
    return 1;
}
