#ifndef SERVER_H
#define SERVER_H

#include "common.h"

// Estrutura do jogador
struct player {
    int currentX; 
    int currentY;
    int completeMaze[10][10];
    int currentMaze[10][10];
    int numCols;
    int numRows;
};


// Declaração das funções
void initPlayer(struct player *p);
void update(struct action* responseAction);
void findEntrance(struct player* Player);
int move(struct action receivedAction, struct action* responseAction, struct player* Player);
void printMaze(int maze[10][10], int numRows, int numCols);
void loadMazeFromFile(const char *filePath, struct player* Player);
void allowedMoves(struct action* responseAction, struct player Player);
void updateCoord(int d, struct player* Player);
void map(struct action* responseAction, struct player Player);
void fillMapAfterWin(struct action* responseAction, struct player Player);
void copyMaze(struct player* Player);
void resetPlayer(struct player* p);
void getHint(struct player Player, struct action* responseAction);
int findPath(int x, int y, int maze[10][10], int visited[10][10], struct player Player, int *hint_moves, int *move_count);
int isValid(int x, int y, int maze[10][10], int visited[10][10], int numCols, int numRows);

#endif 
