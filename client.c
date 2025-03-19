// client.c
#include "clientUtils.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 51511


int main(int argc, char *argv[]) {
    if (argc != 3) {
        return 1;
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);
    int sock = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in6 server_addr6;

    // Verifica se o endereço é IPv6
    if (strchr(server_ip, ':')) {
        sock = socket(AF_INET6, SOCK_STREAM, 0);
        if (sock < 0) {
            return 1;
        }

        memset(&server_addr6, 0, sizeof(server_addr6));
        server_addr6.sin6_family = AF_INET6;
        server_addr6.sin6_port = htons(port);
        if (inet_pton(AF_INET6, server_ip, &server_addr6.sin6_addr) <= 0) {
            return 1;
        }

        if (connect(sock, (struct sockaddr *)&server_addr6, sizeof(server_addr6)) < 0) {
            close(sock);
            return 1;
        }
    } else {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            return 1;
        }

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
            return 1;
        }

        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            close(sock);
            return 1;
        }
    }

    int gameRunning = 1;
    int won = 0; 
    int started = 0;
    int validMoves[100] = {0};
    while (gameRunning) {

        // Preparar e enviar a mensagem de requisição
        char request[10];
        scanf("%s", request);

        struct action requestAction; 
        
        struct action receivedAction; 
        initAction(&requestAction);

        // Obter o tipo de requisição
        requestAction.type = mapCommand(request);

        if (requestAction.type < 0) {
            printf("error: command not found\n");
            continue;
        } else if (started) {
            if (!won) {
                if (requestAction.type == 1) {
                    requestAction.moves[0] = defineMove(request);
                    if (moveIsValid(requestAction.moves[0], validMoves)) {
                        // Enviar mensagem para mover
                        send(sock, &requestAction, sizeof(requestAction), 0);
                    } else {
                        printf("error: you cannot go this way\n");
                        continue;
                    } 
                } else if (requestAction.type != 0) { // map, reset, exit, hint
                    send(sock, &requestAction, sizeof(requestAction), 0);
                } else {
                    continue;
                }
            } else if (won) {
                if (requestAction.type == 6 ) {
                    send(sock, &requestAction, sizeof(requestAction), 0); 
                    won = 0; 
                } else if (requestAction.type == 7) {
                    send(sock, &requestAction, sizeof(requestAction), 0); 
                    break;
                } else {
                    continue;
                }
            }
        } else if (!started) {
            if (requestAction.type == 0 ) {
                // Enviar mensagem para iniciar o jogo
                send(sock, &requestAction, sizeof(requestAction), 0);
                started = 1;
            } else {
                printf("error: start the game first\n");
                continue;
            }
        }

        // Receber a resposta do servidor
        char buffer[sizeof(struct action)] = {0};
        ssize_t valread = read(sock, buffer, sizeof(struct action));
        
        if (valread > 0) {
            memcpy(&receivedAction, buffer, sizeof(struct action));
            
            if (requestAction.type == 1 || requestAction.type == 0 || requestAction.type == 6) {
                for (int i = 0; i < 100; i++) {
                    validMoves[i] = receivedAction.moves[i];
                }
            }
            if (requestAction.type == 3) {
                printHint(receivedAction.moves);
            } else {
                printMoves(receivedAction.moves);
            }

            if(receivedAction.type == 5) {
                printf("You escaped! \n");
                won = 1;
            }
            printMaze(receivedAction);
            
        } else {
            gameRunning = 0;
        }
    }
    close(sock);
    return 0;
}
