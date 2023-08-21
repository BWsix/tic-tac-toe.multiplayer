#include <bits/types/struct_timeval.h>
#include <netdb.h>
#include <stddef.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>

#define BUTTON_IMPLEMENTATION
#include "button.h"
#include "dbg.h"
#include "raylib.h"
#define TIC_TAC_TOE_IMPLEMENTATION
#include "tic-tac-toe.h"

int main(int argc, char **argv) {
  check(argc > 2, "Usage: %s <hostname> <port>", argv[0]);

  struct addrinfo hints = {0};
  hints.ai_socktype = SOCK_STREAM;
  struct addrinfo *server_ai;
  int r = getaddrinfo(argv[1], argv[2], &hints, &server_ai);
  check(r == 0, "Failed to resolve hostname: %s, port: %s", argv[1], argv[2]);
  int server_fd = socket(server_ai->ai_family, server_ai->ai_socktype,
                         server_ai->ai_protocol);
  check(server_fd != -1, "Failed to create socket");
  r = connect(server_fd, server_ai->ai_addr, server_ai->ai_addrlen);
  check(r == 0, "Failed to connect to server");

  Cell me;
  int bytes_received = recv(server_fd, &me, 1, 0);
  send(server_fd, "seaman", 1, 0);
  check(bytes_received != -1, "Failed to receive message from server");

  fd_set fdset;
  FD_ZERO(&fdset);
  FD_SET(server_fd, &fdset);
  struct timeval interval = {
      .tv_sec = 0,
      .tv_usec = 1000,
  };

  dbg("\n\n\n\n\n\nWaiting for your opponent...\n\n\n\n");
  dbg("I am %c", me);

  char buf[256];
  recv(server_fd, buf, sizeof(buf), 0);
  send(server_fd, " ", 1, 0);

  const int buttonWidth = 180;
  const int gapWidth = 20;
  const int boardWidth = 3;
  const int boardHeight = 3;
  const int screenWidth = (gapWidth + buttonWidth) * boardWidth + gapWidth;
  const int screenHeight =
      (gapWidth + buttonWidth) * boardHeight + gapWidth * 2 + 100;

  InitWindow(screenWidth, screenHeight, "Tic-Tac-Toe.multiplayer");

  Rectangle buttons[9];
  for (size_t r = 0; r < boardHeight; r++) {
    for (size_t c = 0; c < boardWidth; c++) {
      Rectangle button = {.x = buttonWidth * c + gapWidth * (c + 1),
                          .y = buttonWidth * r + gapWidth * (r + 1),
                          .width = buttonWidth,
                          .height = buttonWidth};
      buttons[r * boardWidth + c] = button;
    };
  };

  Cell cells[9];
  for (int i = 0; i < 9; i++) {
    cells[i] = Cell_Empty;
  }
  bool state_my_turn = false;
  bool state_i_won = false;
  bool state_i_died = false;
  bool state_game_tie = false;
  SetTargetFPS(120);
  while (!WindowShouldClose()) {

    fd_set reads = fdset;
    select(server_fd + 1, &reads, NULL, NULL, &interval);

    if (FD_ISSET(server_fd, &reads)) {
      bytes_received = recv(server_fd, buf, sizeof(buf), 0);
      send(server_fd, " ", 1, 0);
      check(bytes_received != -1, "Failed to receive message from server");
      dbg("Received %d bytes: %.*s", bytes_received, bytes_received, buf);
      memcpy(cells, buf, sizeof(cells));

      bytes_received = recv(server_fd, &state_my_turn, 1, 0);
      check(bytes_received != -1, "Failed to receive message from server");
      dbg("Received %d bytes: %d", bytes_received, state_my_turn);
      if (!state_my_turn) {
        send(server_fd, " ", 1, 0);
      }
    }

    if (Wins(cells, me)) {
      state_i_won = true;
    } else if (Loses(cells, me)) {
      state_i_died = true;
    } else if (IsBoardFull(cells)) {
      state_game_tie = true;
    }

    for (int i = 0; i < 9; i++) {
      ButtonState state = GetButtonState(buttons[i], MOUSE_BUTTON_LEFT);
      if (!state_my_turn || state_game_tie || state_i_died || state_i_won) {
        break;
      }
      if (state == BUTTON_ACTIVATED && cells[i] == Cell_Empty) {
        send(server_fd, &i, sizeof(int), 0);
        state_my_turn = false;
      }
    }

    BeginDrawing();
    ClearBackground(GetColor(0x101010ff));

    char *message = state_my_turn ? "Your turn" : "Waiting";
    if (state_i_won) {
      message = "You won!";
    } else if (state_i_died) {
      message = "You died!";
    } else if (state_game_tie) {
      message = "Tie!";
    }
    DrawText(message, gapWidth, screenHeight - gapWidth - 100, 100, RED);

    for (int i = 0; i < 9; i++) {
      Rectangle button = buttons[i];
      ButtonState state = GetButtonState(button, MOUSE_BUTTON_LEFT);
      Cell cell = cells[i];

      if (state == BUTTON_HOVERED && cell == Cell_Empty) {
        DrawRectangleRec(button, GetColor(0x505050ff));
      } else {
        DrawRectangleRec(button, GetColor(0x303030ff));
      }

      char message[] = " ";
      message[0] = cell;
      DrawText(message, button.x + 35, button.y + 10, button.height, RAYWHITE);
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
error:
  printf("\n////////////////////ERROR////////////////////\n");
  return 1;
}
