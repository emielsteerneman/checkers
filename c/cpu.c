#include "main.h"
#include "board.h"
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int log = 1;
int logv = 1;

void printStruct(RecMove m){
    printf("RecMove: nnode = %i, score = %i, nmoves = %i | ", m.nnode, m.score, m.nmoves);
    for(int z = m.nmoves - 1; z >= 0; z--){
        printf("%i %i -> %i %i | ", m.moves[z].y, m.moves[z].x, m.moves[z]._y, m.moves[z]._x);
    }
    printf("\n");
}

void cpu_move(int board[], int piece){

	if(log) printf("\ncpu making move\n");

    Move moves[12*4];
    int nmoves = getMoves(board, moves, piece);
	if(log) printf("%i moves found\n", nmoves);

	RecMove bestMoves[100];
	int nbestMoves = 0;
    int nnode = 0;

    int z;
    for(z = 0; z < nmoves; z++){
		cpu_recursive_move(board, moves[z], 0, bestMoves, &nbestMoves, &nnode);
    }

	int r = rand() % nbestMoves;
    RecMove move = bestMoves[r];

	if(log){ // Print moves
        printf("\nnbestMoves = %i\n", nbestMoves);
        for(z = 0; z < nbestMoves; z++){
            printf("  %i: ", z);
            printStruct(bestMoves[z]);
        }

        printf("Chosen move %i | ", r);
        Move m = move.moves[move.nmoves-1];
        printf("%i %i -> %i %i", m.y, m.x, m._y, m._x);
        for(z = move.nmoves - 2; z >= 0; z--){
            printf(" -> %i %i", move.moves[z]._y, move.moves[z]._x);
        }
        printf("\n");
	}

    // Make moves
    for(z = move.nmoves - 1; z >= 0; z--){
        Move m = move.moves[z];
        makeMove(board, m, moves);

		if(log) printf("\nmaking move %i %i -> %i %i", m.y, m.x, m._y, m._x);
		if(log) printBoard(board);
    }
    if(!log){
        printf("\ncpu | ");
        Move m = move.moves[move.nmoves-1];
        printf("%i %i -> %i %i", m.y, m.x, m._y, m._x);
        for(z = move.nmoves - 2; z >= 0; z--){
            printf(" -> %i %i", move.moves[z]._y, move.moves[z]._x);
        }
        printf("\n");
    }
}

void cpu_recursive_move(int _board[], Move move, int level, RecMove bestMoves[], int* nbestMoves, int* nnode){
	int r = rand() % 1000;
	int n;
	int nodeId = *nnode;
    *nnode = *nnode + 1;

	// Whitespace
	char ws[level+1];
	for(n = 0; n < level; n++){ ws[n] = ' '; }
	ws[level] = '\0';

	if(log) printf("%s== %i == %i %i -> %i %i ====\n", ws, nodeId, move.y, move.x, move._y, move._x);

	// Copy board to new board
	int piece = _board[move.y * SIZE + move.x];
	int board[SIZE * SIZE];
	for(n = 0; n < SIZE*SIZE; n++)	board[n] = _board[n];

	// Make move, get followup moves
	Move moves[3];	                            // Hit or Hop, can't go back to previous
    int nmoves = makeMove(board, move, moves);  // Make move, get followup moves

	RecMove newMove;

    if(nmoves == 0){	// No more followup moves. End reached
		int score = cpu_calcScore(board, piece);		// Calculate board score

		// Initialize newMove
		newMove.nmoves = 1;                             // set nmoves
		newMove.score = score;	                        // set score
		newMove.nnode = nodeId;                         // set nnode
		newMove.moves[0] = move;						// Add first move

        if(*nbestMoves > 0){    // If there are already best moves
			int curBestScore = bestMoves[*nbestMoves - 1].score;	// Get score of current best score
			if(curBestScore < score){								// If new score is better
				if(log) printf("%s  Better %i > %i\n", ws, score, curBestScore);
				bestMoves[0] = newMove;                                     // Reset bestMoves[]
				*nbestMoves = 1;                                            // Reset nbestMoves
			}else
			if(curBestScore == score){                              // If new score is equal
                if(log) printf("%s  Same %i == %i\n", ws, score, curBestScore);
                bestMoves[*nbestMoves] = newMove;                           // Add move to bestMoves[]
                *nbestMoves = *nbestMoves + 1;                              // Increment nbestMoves
			}else{                                                  // If new score is worse
				if(log) printf("%s  Worse %i < %i\n", ws, score, curBestScore);
			}
			// printBoard(board);
        }else
        if(*nbestMoves == 0){   // If this is the first move
			if(log) printf("%s  First %i. %i\n", ws, newMove.nnode, score);
			bestMoves[0] = newMove;         // Reset bestMoves[]
			*nbestMoves = 1;                // Reset nbestMoves
        }

	}else{  // One or more folowup moves
		int bestScore = -1000;

        // Try every followup move
		for(n = 0; n < nmoves; n++){
			cpu_recursive_move(board, moves[n], level + 4, bestMoves, nbestMoves, nnode);
		}

        // Add own move to each move in bestMoves[], and increment their nbestMoves
        //printf("%siterating.. ", ws);
        //printf("nnode = %i ", nodeId);
        //printf("nbestMoves = %i \n", *nbestMoves);

        for(n = 0; n < *nbestMoves; n++){
            if(nodeId < bestMoves[n].nnode){
                //printf("%s Added move to %i\n", ws, bestMoves[n].nnode);
                bestMoves[n].moves[bestMoves[n].nmoves] = move;
                bestMoves[n].nmoves++;
            }//else{
            //    printf("%s %i > %i\n", ws, nodeId, bestMoves[n].nnode);
            //}
        }

    }

    return newMove;
}

int cpu_calcScore(int board[], int piece){
    int score = 0;
	int other = ~piece & (b | w);
	int nmoves = 0;
	Move moves[12*4];

	int y; int x;
    for(y = 0; y < SIZE; y++){
        for(x = 0; x < SIZE; x++){

            int p = board[y * SIZE + x];

            if(p & piece) score ++;
            if(p & piece && p & IS_KING) score++;

            if(p & other) score --;
            if(p & other && p & IS_KING) score--;

            //printf("\tat %i %i score %i\n", y, x, score);
            //printf("\t  p %i, other %i, p & other %i, >> %i\n", p, other, p & other, (p & other) >> (other - 1));
        }
    }

    score -= calcEndangered(board, piece);
    score += calcEndangered(board, other);

    //nmoves = getMoves(board, moves, piece);
    //if(nmoves > 0 && abs(moves[0].y - moves[0]._y) == 2) score += nmoves;

	//nmoves = getMoves(board, moves, other);
    //if(nmoves > 0 && abs(moves[0].y - moves[0]._y) == 2) score -= nmoves;

    return score;
}








int calcEndangered(int board[], int piece){
	if(logv) printf("Calculating endangered for %c\n", pStr[piece]);
    int other = ~piece & (b | w);
    int andBoard[SIZE*SIZE];
    int n;

     // get possible moves
    Move moves[12];
    int nmoves = getMoves(board, moves, other);

    if(nmoves == 0 || abs(moves[0].y - moves[0]._y != 2))
        return 0;

    // Copy board
    for(n = 0; n < SIZE*SIZE; n++){
        andBoard[n] = board[n] & piece;
    }

    for(n = 0; n < nmoves; n++){
		werew(board, moves[n], 0, andBoard);
    }

    int total = 0; int left = 0;
    for(n = 0; n < SIZE*SIZE; n++){
        if(board[n]   & piece) total++;
        if(andBoard[n]& piece) left++;
    }
    if(logv) printf("\ntotal = %i, left = %i, endangered = %i\n", total, left, total-left);
    return total - left;

}

void werew(int _board[], Move move, int level, int andBoard[]){  ////// ADD _ANDBOARD
	int r = rand() % 1000;
	int n;

	// Whitespace
	char ws[level+1];
	for(n = 0; n < level; n++){ ws[n] = ' '; }
	ws[level] = '\0';
	//printf("%s==== %i %i -> %i %i ====\n", ws, move.y, move.x, move._y, move._x);

	// Copy board to new board
	int piece = _board[move.y * SIZE + move.x];
	int board[SIZE * SIZE];
	for(n = 0; n < SIZE*SIZE; n++)	board[n] = _board[n];

	// Make move, get followup moves
	Move moves[3];	                            // Hit or Hop, can't go back to previous
    int nmoves = makeMove(board, move, moves);  // Make move, get followup moves

    // INSERT LOGIC HERE
    if(nmoves == 0){	// No more followup moves. End reached

        //printf("%sAt end!\n", ws);
        //printBoard(board);
        //printBoard(andBoard);
        for(n = 0; n < SIZE * SIZE; n++){
            andBoard[n] = andBoard[n] & board[n];
        }
        //printBoard(andBoard);





	}else{  // One or more folowup moves
		// Try every followup move
		for(n = 0; n < nmoves; n++){
			werew(board, moves[n], level + 4, andBoard);
		}
    }
}












