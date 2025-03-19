#include "clientUtils.h"

void initAction(struct action* requestAction) {
    memset(requestAction->moves, 0, sizeof(requestAction->moves));
    memset(requestAction->board, 6, sizeof(requestAction->board));
    requestAction->type = -1; // Tipo de resposta
}

void printHint(const int hintMoves[]) {
    int i = 0;
    if (hintMoves[i] != 0) {
        printf("Hint: ");
        while(hintMoves[i] != 0) {
            if (hintMoves[i] == 1) printf("up");
            else if (hintMoves[i] == 2) printf("right");
            else if (hintMoves[i] == 3) printf("down");
            else if (hintMoves[i] == 4) printf("left");
            
            i++;
            if (hintMoves[i] != 0) { 
                printf(", ");
            } else { printf(".\n");}
        }
    }
}

// Função para mapear comandos de entrada para tipos de ações
int mapCommand(const char *command) {
    if (strcmp(command, "start") == 0) {
        return 0;
    } else if (strcmp(command, "up") == 0 || strcmp(command, "down") == 0 ||
               strcmp(command, "left") == 0 || strcmp(command, "right") == 0) {
        return 1;
    } else if(strcmp(command, "map") == 0) {
        return 2;
    } else if(strcmp(command, "hint") == 0) {
        return 3;
    } else if(strcmp(command, "reset") == 0) {
        return 6;
    } else if(strcmp(command, "exit") == 0 ) {
        return 7; // Retornar um valor padrão
    }
    return -1;
}

int testValidBoard(int b) {
    if (b >= 0 && b < 6) {
        return 1;
    }
    return 0;
}

int moveIsValid(int m, const int validMoves[]) {
    int i = 0;
    while (validMoves[i] != 0) {
        if (m == validMoves[i]) {
            return 1;
        }
        i++;
    }
    return 0;
}

char mapMazeSymbols(int i) {
    switch(i){
        case 0: return '#'; 
        case 1: return '_';
        case 2: return '>';
        case 3: return 'X';
        case 4: return '?';
        case 5: return '+';
    }
    return 0;
}

void printMaze(struct action receivedAction) {
    int i = 0;
    int j = 0;

    while (testValidBoard(receivedAction.board[i][j])) {
        while (testValidBoard(receivedAction.board[i][j])) {
            
            printf("%c\t", mapMazeSymbols(receivedAction.board[i][j]));
            j++;
        }
        printf("\n");
        i++;
        j = 0;
    }
}

int defineMove(const char *command) {
    if (strcmp(command, "up") == 0) {
        return 1;
    } else if (strcmp(command, "right") == 0) {
        return 2;
    } else if (strcmp(command, "down") == 0) {
        return 3;
    } else if (strcmp(command, "left") == 0) {
        return 4;
    }
    return -1;
}

// Função para imprimir os movimentos possíveis
void printMoves(const int moves[]) {
    
    if (moves[0] != 0) {
        printf("Possible moves: ");
        int first = 1; // Usar uma flag para formatar a saída

        for (int i = 0; i < 100; i++) {
            if (moves[i] == 0) break; // Parar quando não houver mais movimentos

            if (!first) {
                printf(", ");
            }
            switch (moves[i]) {
                case 1: printf("up"); break;
                case 2: printf("right"); break;
                case 3: printf("down"); break;
                case 4: printf("left"); break;
            }
            first = 0; // Depois do primeiro movimento, não é mais o primeiro
        }
        printf(".\n");
    }
}
