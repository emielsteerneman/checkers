extern int SIZE;
extern int E;
extern int b;
extern int B;
extern int w;
extern int W;
extern int IS_KING;
extern int IS_PIECE;

extern int log;
extern int logv;
extern int logvv;

typedef struct {
	int y;
	int x;
	int _y;
	int _x;
} Move;

typedef struct {
	Move moves[12];	// A piece can make 12 moves max
	int nmoves;
	int score;
	int nnode;
} RecMove;
