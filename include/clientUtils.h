#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

#include <stdio.h>
#include <string.h>
#include "common.h"

int mapCommand(const char *command);
void printMoves(const int moves[]);
int testValidBoard(int b);
char mapMazeSymbols(int i);
void printMaze(struct action receivedAction);
int defineMove(const char *command);
void initAction(struct action* requestAction);
int moveIsValid(int m, const int validMoves[]);
void printHint(const int hintMoves[]);

#endif 