# servidor-de-labirintos
TP1 de Redes de Computadores

O objetivo deste trabalho é criar uma versão de um jogo de Exploração de Labirintos
que permitirá a interação entre um cliente e um servidor usando sockets em
linguagem C. O servidor será projetado para acomodar a conexão de um único
cliente, proporcionando uma experiência de jogo remoto.
No início do jogo, o cliente envia um comando para iniciar a partida e recebe do
1
servidor quais movimentos o jogador pode fazer. O cliente pode então enviar para
qual direção quer ir, e o servidor vai atualizar a posição do jogador (caso o
movimento seja válido) e retornar quais novos movimentos ele pode fazer. O cliente
pode também requisitar ver o mapa, ação que faz com que o servidor retorne o
labirinto para o usuário com todas as regiões ocultas, exceto pelas que o jogador já
sabe o que são (como caminhos livres ou muros).
O jogo é vencido quando o cliente encontra a saída. O servidor informa ao cliente
que o jogo foi vencido, e revela o labirinto com todas as células reveladas (mesmo
se o jogador não descobriu todas). A partida é encerrada com sucesso.
Contudo, o cliente pode enviar um comando de encerrar o jogo a qualquer
momento, onde após enviar a mensagem vai ser desconectado, e o servidor vai
resetar o estado do jogo.
Por fim, como pontuação extra, deve ser implementado um comando extra de dica,
onde, dado a posição atual do jogador, o servidor deve retornar uma lista de
comandos que resultem num caminho válido do jogador até a saída. Esse caminho
não precisa necessariamente ser o mais curto, apenas um que leve o jogador até o
final.
