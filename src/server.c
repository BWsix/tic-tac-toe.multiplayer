#include <netdb.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>

#include "dbg.h"
#define TIC_TAC_TOE_IMPLEMENTATION
#include "tic-tac-toe.h"

int main(int argc, char **argv) {
  check(argc > 1, "Usage: %s <port>", argv[0]);

  struct addrinfo hints = {0};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo *server_ai;
  int r = getaddrinfo("0.0.0.0", argv[1], &hints, &server_ai);
  check(r == 0, "Failed to resolve address info");

  int server_fd = socket(server_ai->ai_family, server_ai->ai_socktype,
                         server_ai->ai_protocol);
  check(server_fd != -1, "Failed to create socket");

  r = bind(server_fd, server_ai->ai_addr, server_ai->ai_addrlen);
  check(r == 0, "Failed to bind");

  r = listen(server_fd, 2);
  check(r == 0, "Failed to listen");

  dbg("waiting for first palyer");
  struct sockaddr_storage client_addr;
  socklen_t socklen = sizeof(client_addr);
  int clientO_fd = accept(server_fd, (struct sockaddr *)&client_addr, &socklen);

  char buf[256];

  send(clientO_fd, "O", sizeof(char), 0);
  recv(clientO_fd, buf, sizeof(buf), 0);

  dbg("waiting for second palyer");
  int clientX_fd = accept(server_fd, (struct sockaddr *)&client_addr, &socklen);

  send(clientX_fd, "X", sizeof(char), 0);
  recv(clientX_fd, buf, sizeof(buf), 0);

  Cell cells[9];
  for (size_t i = 0; i < 9; i++) {
    cells[i] = Cell_Empty;
  }

  send(clientO_fd, " ", 1, 0);
  recv(clientO_fd, buf, sizeof(buf), 0);
  send(clientX_fd, " ", 1, 0);
  recv(clientX_fd, buf, sizeof(buf), 0);

  for (int i = 0; i < 10; i++) {
    send(clientO_fd, cells, sizeof(cells), 0);
    recv(clientO_fd, buf, sizeof(buf), 0);
    send(clientX_fd, cells, sizeof(cells), 0);
    recv(clientX_fd, buf, sizeof(buf), 0);

    Cell current_player = (i % 2 == 0) ? Cell_O : Cell_X;
    int move = -1;
    bool Os_turn = current_player == Cell_O;
    bool Xs_turn = current_player == Cell_X;

    send(clientO_fd, &Os_turn, sizeof(bool), 0);
    send(clientX_fd, &Xs_turn, sizeof(bool), 0);
    if (Os_turn) {
      recv(clientO_fd, &move, sizeof(int), 0);
      recv(clientX_fd, buf, sizeof(buf), 0);
      dbg("O: %d", move);
      cells[move] = Cell_O;
    }
    if (Xs_turn) {
      recv(clientO_fd, buf, sizeof(buf), 0);
      recv(clientX_fd, &move, sizeof(int), 0);
      dbg("X: %d", move);
      cells[move] = Cell_X;
    }
  }

  freeaddrinfo(server_ai);
  r = shutdown(server_fd, SHUT_RDWR);
  check(r == 0, "Failed to shut down");
  return 0;
error:
  return 1;
}
