#ifndef STOCKFISH_H
#define STOCKFISH_H

// Function prototype
string askStockfish(vector<string> moves, int fd1[2], int fd2[2]);
bool testNext(vector<string> moves, string nextMove, int fd1[2], int fd2[2]);
bool testCheck(vector<string> moves, int fd1[2], int fd2[2]);
bool testMove(vector<string> moves, int fd1[2], int fd2[2]);

#endif

