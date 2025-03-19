#include "serverUtils.h"


void initPlayer(struct player* p) {
    p->numCols = p->numRows = 0;
    p->currentX = p->currentY = -1;
    for(int i=0; i<10; i++) {
        for(int j=0; j<10; j++){
            p->completeMaze[i][j] = 6;
            p->currentMaze[i][j] = 6;
        }
    }
}

void resetPlayer(struct player* p) {
    for(int i=0; i<10; i++) {
        for(int j=0; j<10; j++){
            p->currentMaze[i][j] = 6;
        }
    }
    copyMaze(p);
    findEntrance(p);
}

void update(struct action* responseAction) {
    // Preparar a resposta com a estrutura action
    memset(responseAction->moves, 0, sizeof(responseAction->moves));
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            responseAction->board[i][j] = 6;
        }
    }
    responseAction->type = 4; // Tipo de resposta
}

// Função para percorrer e imprimir os valores diretamente em volta de (i, j), incluindo diagonais
void traverseAdjacentCells(struct player* Player) {
    // Definindo os deslocamentos para todas as direções ao redor (incluindo diagonais)
    int directions[8][2] = {
        {-1, 0}, // acima
        {1, 0},  // abaixo
        {0, -1}, // esquerda
        {0, 1},  // direita
        {-1, -1}, // diagonal superior esquerda
        {-1, 1},  // diagonal superior direita
        {1, -1},  // diagonal inferior esquerda
        {1, 1}    // diagonal inferior direita
    };

    // Percorrendo as direções
    for (int d = 0; d < 8; d++) {
        int new_i = Player->currentY + directions[d][0];
        int new_j = Player->currentX + directions[d][1];

        // Verificando se a nova posição está dentro dos limites da matriz
        if (new_i >= 0 && new_i < Player->numRows && new_j >= 0 && new_j < Player->numCols) {
            Player->currentMaze[new_i][new_j] = Player->completeMaze[new_i][new_j];
        }
    }
}

void fillMapAfterWin(struct action* responseAction, struct player Player) {
    for(int i = 0; i < Player.numRows; i++) {
        for(int j = 0; j< Player.numCols; j++) {
            responseAction->board[i][j] = Player.completeMaze[i][j];
        }
    }
}

// Busca as coordenadas iniciais do jogador
void findEntrance(struct player* Player) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (Player->completeMaze[i][j] == 2) {
                Player->currentX = j;
                Player->currentY = i;
                Player->currentMaze[i][j] = 5;
                break;
            }
        }
    }

    // atualiza o mapa do jogador
    traverseAdjacentCells(Player);
}

// Verifica os movimentos que o jogador pode fazer
void allowedMoves(struct action* responseAction, struct player Player) {
    // Determinar movimentos válidos para o jogador
    int x = Player.currentX;
    int y = Player.currentY;

    int moveIndex = 0;

    // Verifica se pode mover para cima
    if (y > 0 && Player.currentMaze[y - 1][x] != 0) {
        responseAction->moves[moveIndex++] = 1; // Cima
    }

    // Verifica se pode mover para a direita
    if (x < Player.numCols - 1 && Player.currentMaze[y][x + 1] != 0) {
        responseAction->moves[moveIndex++] = 2; // Direita
    }

    // Verifica se pode mover para baixo
    if (y < Player.numRows - 1 && Player.currentMaze[y + 1][x] != 0) {
        responseAction->moves[moveIndex++] = 3; // Baixo
    }

    // Verifica se pode mover para a esquerda
    if (x > 0 && Player.currentMaze[y][x - 1] != 0) {
        responseAction->moves[moveIndex++] = 4; // Esquerda
    }

    // Preencher o restante do vetor moves com 0
    for (int i = moveIndex; i < 100; i++) {
        responseAction->moves[i] = 0;
    }
}

// Atualiza coordenadas do jogador de acordo com direção 
void updateCoord(int d, struct player* Player) {
    int oldX = Player->currentX;
    int oldY = Player->currentY;
    
    // retorna o valor original de onde o jogador estava antes
    Player->currentMaze[oldY][oldX] = Player->completeMaze[oldY][oldX];

    if (d == 1) { // cima
        Player->currentY--;
    } else if (d == 3) { // baixo
        Player->currentY++;
    } else if(d == 4) { // esquerda
        Player->currentX--;
    } else if(d == 2) { // direita
        Player->currentX++;
    }
}

int move(struct action receivedAction, struct action* responseAction, struct player* Player) {
    allowedMoves(responseAction, *Player);
    int i = 0;
    int d = receivedAction.moves[0];
    while(responseAction->moves[i] != 0) {
        if (responseAction->moves[i] == d) {
            updateCoord(d, Player); // atualiza coordenadas
            break;
        }
        i++;
    }
    // atualiza o mapa do jogador
    traverseAdjacentCells(Player);

    memset(&responseAction->moves, 0, sizeof(responseAction->moves));
    allowedMoves(responseAction, *Player);

    // nova posição do jogador
    Player->currentMaze[Player->currentY][Player->currentX] = 5;
    if(Player->completeMaze[Player->currentY][Player->currentX] == 3) {
        fillMapAfterWin(responseAction, *Player);
        return 1;
    }
    return 0;
}

void copyMaze(struct player* Player) {
    for(int i = 0; i < Player->numRows; i++) {
        for(int j = 0; j < Player->numCols; j++) {
            Player->currentMaze[i][j] = 4;
        }
    }
}

void printMaze(int maze[10][10], int numRows, int numCols) {
    printf("Labirinto carregado:\n");
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            printf("%d ", maze[i][j]);
        }
        printf("\n");
    }
}

void map(struct action* responseAction, struct player Player) {
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++){
            responseAction->board[i][j] = Player.currentMaze[i][j];
        }
    }
}

void loadMazeFromFile(const char *filePath, struct player* Player) {
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char line[256];

    int i = 0; int j = 0;
    // Lê o arquivo linha por linha
    while (fgets(line, sizeof(line), file)) {
        // Remove a nova linha do final da linha, se existir
        line[strcspn(line, "\n")] = '\0';

        // Quebra a linha em tokens usando espaço como delimitador
        char *token = strtok(line, " ");
        while (token != NULL) {
            Player->completeMaze[i][j] = atoi(token);
            token = strtok(NULL, " "); // Próximo token
            j++;
            if(j > Player->numCols) {
                Player->numCols = j;
            }
        }
        i++;
        j = 0;
    }
    Player->numRows = i--;

    fclose(file);

    copyMaze(Player);
}

int isValid(int x, int y, int maze[10][10], int visited[10][10], int numCols, int numRows) {
    return (x >= 0 && y >= 0 && x < numCols && y < numRows && maze[y][x] >=  1 && maze[y][x] <=3 && !visited[y][x]);
}

int findPath(int x, int y, int maze[10][10], int visited[10][10], struct player Player, int *hint_moves, int *move_count) {
    // Se encontrou a saída
    if (maze[y][x] == 3) {
        return 1; // Caminho encontrado
    }

    // Marca como visitado
    visited[y][x] = 1;

    // Direções: cima, direita, baixo, esquerda
    int directions[4][2] = {
        {-1, 0}, // Cima
        {0, 1},  // Direita
        {1, 0},  // Baixo
        {0, -1}  // Esquerda
    };

    // Para cada direção
    for (int i = 0; i < 4; i++) {
        int ny = y + directions[i][0];
        int nx = x + directions[i][1];

        if (isValid(nx, ny, maze, visited, Player.numCols, Player.numRows)) {
            // Adiciona o movimento ao vetor de dicas
            hint_moves[*move_count] = i + 1; 
            (*move_count)++;

            // Chamada recursiva
            if (findPath(nx, ny, maze, visited, Player, hint_moves, move_count)) {
                return 1; // Caminho encontrado
            }

            // Remove o movimento (backtracking)
            hint_moves[*move_count] = 0;
            (*move_count)--;
        }
    }

    // Marca como não visitado (backtracking)
    visited[y][x] = 0;

    return 0; // Nenhum caminho encontrado
}

void getHint(struct player Player, struct action* responseAction) {
    int visited[10][10] = {0};       // Matriz de visitados       
    int move_count = 0;              // Contador de movimentos

    // Inicia a busca
    if (findPath(Player.currentX, Player.currentY, Player.completeMaze, visited, Player, responseAction->moves, &move_count)) {
        return;
    } 
    memset(responseAction->moves, 0, sizeof(responseAction->moves));
}


