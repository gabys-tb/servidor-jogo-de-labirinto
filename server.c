// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "include/serverUtils.h"

#define PORT 51511

struct player Player;

void handleClient(int client_socket) {
    printf("client connected\n");
    int gameRunning = 1;
    int started = 0;

    while (gameRunning) {
        char buffer[sizeof(struct action)] = {0};
        ssize_t valread = read(client_socket, buffer, sizeof(struct action));
        struct action receivedAction;
        memcpy(&receivedAction, buffer, sizeof(struct action));

        // Preparar a resposta com a estrutura action
        struct action responseAction;
        update(&responseAction);

        if (!started) {
            // Verificar se o tipo de mensagem é para iniciar o jogo
            if (valread > 0 && receivedAction.type == 0) {
                printf("starting new game\n");
                started = 1;
                allowedMoves(&responseAction, Player);
                // Enviar a estrutura de resposta ao cliente
                send(client_socket, &responseAction, sizeof(responseAction), 0);
                continue;
            }
        } else if (valread > 0) {
            // Verificar se o tipo de mensagem é para encerrar o jogo
            if (receivedAction.type == 7) {
                printf("client disconnected\n");
                resetPlayer(&Player);
                break;
            } else if (receivedAction.type == 1) {
                if(move(receivedAction, &responseAction, &Player)) {
                    responseAction.type = 5;
                }
            } else if (receivedAction.type == 2) {
                map(&responseAction, Player);
            } else if (receivedAction.type == 6) {
                resetPlayer(&Player);
                allowedMoves(&responseAction, Player);
                printf("starting new game\n");
            } else if (receivedAction.type == 3) {
                getHint(Player, &responseAction);
            }
            // Enviar a estrutura de resposta ao cliente
            send(client_socket, &responseAction, sizeof(responseAction), 0);
        } else {
            gameRunning = 0;
        }
    }
    close(client_socket);
}

int main(int argc, char *argv[]) {
    if (argc != 5 || strcmp(argv[3], "-i") != 0) {
        return 1;
    }

    initPlayer(&Player);

    char *addressType = argv[1];
    int port = atoi(argv[2]);
    char *inputFilePath = argv[4];

    // Carregar o labirinto do arquivo
    loadMazeFromFile(inputFilePath, &Player);
    findEntrance(&Player);

    int server_fd;
    struct sockaddr_storage address;
    socklen_t addr_len;
    int opt = 1;

    if (strcmp(addressType, "v4") == 0) {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            return 1;
        }

        struct sockaddr_in *addr_in = (struct sockaddr_in *)&address;
        addr_in->sin_family = AF_INET;
        addr_in->sin_addr.s_addr = INADDR_ANY;
        addr_in->sin_port = htons(port);
        addr_len = sizeof(*addr_in);
    } else if (strcmp(addressType, "v6") == 0) {
        server_fd = socket(AF_INET6, SOCK_STREAM, 0);
        if (server_fd == -1) {
            return 1;
        }

        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)&address;
        addr_in6->sin6_family = AF_INET6;
        addr_in6->sin6_addr = in6addr_any;
        addr_in6->sin6_port = htons(port);
        addr_len = sizeof(*addr_in6);
    } else {
        return 1;
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(server_fd, (struct sockaddr *)&address, addr_len) < 0) {
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 1) < 0) {
        close(server_fd);
        return 1;
    }

    while (1) {  // Loop principal para aceitar múltiplos clientes
        int new_socket = accept(server_fd, (struct sockaddr *)&address, &addr_len);
        if (new_socket < 0) {
            
            close(server_fd);
            return 1;
        }

        // Manipular cliente
        handleClient(new_socket);

        // Fechar o socket do cliente após manipulação
        close(new_socket);
    }
    close(server_fd);
    return 0;
}