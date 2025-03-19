#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Estrutura da mensagem
struct action {
    int type;
    int moves[100];
    int board[10][10];
};

#endif