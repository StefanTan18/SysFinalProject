#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  char w[BUFFER_SIZE];
  char r[BUFFER_SIZE];

  while(1){
    printf("Enter data: ");
    fgets(w, BUFFER_SIZE, stdin);
    w[strlen(w) - 1] = '\0';

    write(to_server, w, BUFFER_SIZE);
    read(from_server, r, BUFFER_SIZE);
    printf("Server's Response: %s\n", r);
  }
}
