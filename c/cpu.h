void printStruct(RecMove m);
void cpu_move(int board[], int piece);
void cpu_recursive_move(int _board[], Move move, int level, RecMove bestMoves[], int* nbestMoves, int* nnode);
int cpu_calcScore(int board[], int piece);
int calcEndangered(int board[], int piece);
void cpu_recursive_and(int _board[], Move move, int level, int andBoard[]);
