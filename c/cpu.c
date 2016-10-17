#include "main.h"
#include "board.h"
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printStruct(RecMove m){
    printf("RecMove: nnode = %i, score = %i, nmoves = %i | ", m.nnode, m.score, m.nmoves);
    for(int z = m.nmoves - 1; z >= 0; z--){
        printf("%i %i -> %i %i | ", m.moves[z].y, m.moves[z].x, m.moves[z]._y, m.moves[z]._x);
    }
    printf("\n");
}

int cpu_move(int board[], int piece){

	// Get all moves
    Move moves[12*4];
    int nmoves = getMoves(board, moves, piece);
    if(nmoves == 0)
        return 0;


	if(log) printf("CPU moving piece %c, %i moves found\n", pStr[piece], nmoves);

	RecMove bestMoves[100]; // Should be enough..
	int nbestMoves = 0;
    int nnode = 0;

    // Make every move
    int z;
    for(z = 0; z < nmoves; z++)
		cpu_recursive_move(board, moves[z], 0, bestMoves, &nbestMoves, &nnode);

	// Pick a random move from best moves
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

    return nmoves;
}



void cpu_recursive_move(int _board[], Move move, int level, RecMove bestMoves[], int* nbestMoves, int* nnode){
	int n;
	int nodeId = *nnode;
    *nnode = *nnode + 1;

	// Whitespace
	char ws[level+1];
	for(n = 0; n < level; n++){ ws[n] = ' '; }
	ws[level] = '\0';

	if(level == 0 && logv == 1){
		printf("\n");
	}
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
		int score = cpu_calcScore(board, piece, level+2);		// Calculate board score

		// Initialize newMove
		newMove.nmoves = 1;                             // set nmoves
		newMove.score = score;	                        // set score
		newMove.nnode = nodeId;                         // set nnode
		newMove.moves[0] = move;						// Add first move

        if(*nbestMoves > 0){    // If there are already best moves
			int curBestScore = bestMoves[*nbestMoves - 1].score;	// Get score of current best score
			if(curBestScore < score){								// If new score is better
				if(log) printf("%sBetter %i > %i\n", ws, score, curBestScore);
				bestMoves[0] = newMove;                                     // Reset bestMoves[]
				*nbestMoves = 1;                                            // Reset nbestMoves
			}else
			if(curBestScore == score){                              // If new score is equal
                if(log) printf("%sSame %i == %i\n", ws, score, curBestScore);
                bestMoves[*nbestMoves] = newMove;                           // Add move to bestMoves[]
                *nbestMoves = *nbestMoves + 1;                              // Increment nbestMoves
			}else{                                                  // If new score is worse
				if(log) printf("%sWorse %i < %i\n", ws, score, curBestScore);
			}
			// printBoard(board);
        }else
        if(*nbestMoves == 0){   // If this is the first move
			if(log) printf("%sFirst %i. %i\n", ws, newMove.nnode, score);
			bestMoves[0] = newMove;         // Reset bestMoves[]
			*nbestMoves = 1;                // Reset nbestMoves
        }

	}else{  // One or more folowup moves

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

    return;
}

int cpu_calcScore(int board[], int piece, int level){
    int score = 0; int n;
    piece = piece & IS_PIECE;
	int other = ~piece & (b | w);

	// Whitespace
	char ws[level+1];
	for(n = 0; n < level; n++){ ws[n] = ' '; }
	ws[level] = '\0';

	if(logv) printf("%scpu_calcScore: calculating score for '%c'\n", ws, pStr[piece]);

	int y; int x;
	int _p = 0; int _pk = 0; int _b = 0; int _bk = 0;

    for(y = 0; y < SIZE; y++){
        for(x = 0; x < SIZE; x++){

            int p = board[y * SIZE + x];

            if(p & piece) _p++;
            if(p & piece && p & IS_KING) _pk++;

            if(p & other) _b++;
            if(p & other && p & IS_KING) _bk++;

        }
    }
    score = _p + _pk - _b - _bk;

	if(_p == 0) return -100;
	if(_b == 0) return  100;

    if(logv){
        printf("%scpu_calcScore: current score: %i | p %i | pk %i | b %i | bk %i\n", ws, score, _p, _pk, _b, _bk);

        x = calcEndangered(board, piece, level+2);
        score -= x;
        printf("%scpu_calcScore: endangered for %c: %i | new score: %i\n", ws, pStr[piece], x, score);

        x = calcEndangered(board, other, level+2);
        score += x;
        printf("%scpu_calcScore: endangered for %c: %i | new score: %i\n", ws, pStr[other], x, score);
    }else{
        score -= calcEndangered(board, piece, 0);
        score += calcEndangered(board, other, 0);
    }

	return score;
}








int calcEndangered(int board[], int piece, int level){

    int other = ~piece & (b | w);
    int andBoard[SIZE*SIZE];
    int n;

    // Whitespace
	char ws[level+1];
	for(n = 0; n < level; n++){ ws[n] = ' '; }
	ws[level] = '\0';

	if(logv) printf("%scalcEndangered: Calculating endangered for '%c'\n", ws, pStr[piece]);
    //if(logv) printBoard(board);

	// get possible moves
    Move moves[4*12];
    int nmoves = getMoves(board, moves, other);

	for(n = 0; n < nmoves; n++){
        // cprintf("    move %i: %i %i -> %i %i\n", n, moves[n].y, moves[n].x, moves[n]._y, moves[n]._x);
    }

    if(nmoves == 0 || abs(moves[0].y - moves[0]._y) != 2){
        if(logv) printf("%scalcEndangered: no hit moves available for '%c'\n", ws, pStr[other]);
        //if(nmoves > 0) printf("calcEndangered: %i %i %i %i\n", nmoves, moves[0].y, moves[0]._y, abs(moves[0].y - moves[0]._y));

        return 0;
    }

    if(logv) printf("%scalcEndangered: %i moves available '%c'\n", ws, nmoves, pStr[other]);


    // Copy board
    for(n = 0; n < SIZE*SIZE; n++)
        andBoard[n] = board[n] & piece;

	// Make every move
    for(n = 0; n < nmoves; n++)
		cpu_recursive_and(board, moves[n], level+4, andBoard);

	// Get amount of pieces left on board
    int total = 0; int left = 0;
    for(n = 0; n < SIZE*SIZE; n++){
        if(board[n]   & piece) total++;
        if(andBoard[n]& piece) left++;
    }
    if(logv) printf("%scalcEndangered: total = %i, left = %i, endangered = %i\n", ws, total, left, total-left);
    return total - left;

}

void cpu_recursive_and(int _board[], Move move, int level, int andBoard[]){  ////// ADD _ANDBOARD
	int n;

	// Whitespace
	char ws[level+1];
	for(n = 0; n < level; n++){ ws[n] = ' '; }
	ws[level] = '\0';
	if(logv) printf("%s==== %i %i -> %i %i ====\n", ws, move.y, move.x, move._y, move._x);

	// Copy board to new board
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
			cpu_recursive_and(board, moves[n], level + 4, andBoard);
		}
    }
}

