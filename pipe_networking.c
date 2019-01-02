#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  int f;
  while(1) {
    printf("Server creating wkp...\n");
    mkfifo("wkp", 0644);

    printf("Server receiving client's message...\n");
    char r[HANDSHAKE_BUFFER_SIZE];
    int wkpfd = open("wkp", O_RDONLY);
    if (read(wkpfd, r, HANDSHAKE_BUFFER_SIZE)) {
      printf("Message from client: %s\n", r);
      f = fork();
    }
    if (f) {
      printf("Server removing wkp...\n");
      remove("wkp");
    }
    else {
      printf("Subserver sending an intial acknowledgement message...\n ");
      *to_client = open("ppipe", O_WRONLY);
      write(*to_client, ACK, sizeof(ACK));

      printf("Server receiving client's message...\n");
      char m[BUFFER_SIZE];
      read(wkpfd, m, BUFFER_SIZE);
      printf("Message from client: %s\n", m);
      return wkpfd;
    }
  }
}

/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  printf("Client is creating private FIFO...\n");
  mkfifo("ppipe", 0644);

  printf("Client connecting to server...\n");
  *to_server = open("wkp", O_WRONLY);
  printf("Sending private FIFO name...\n");
  write(*to_server, "ppipe", 6);

  printf("Client receiving server's message...\n");
  char r[HANDSHAKE_BUFFER_SIZE];
  int ppfd = open("ppipe", O_RDONLY);
  read(ppfd, r, HANDSHAKE_BUFFER_SIZE);
  printf("Message from server: %s\n", r);
  printf("Client removing private FIFO...\n");
  remove("ppipe");

  printf("Client sending response to server...\n");
  write(*to_server, ACK, sizeof(ACK));

  return ppfd;
}
