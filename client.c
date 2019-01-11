#include "pipe_networking.h"


/*int main() {

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
*/

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  printf("Enter the number of players(There can't be more than 10 players): ");
  fgets(buffer, sizeof(buffer), stdin);
  *strchr(buffer, '\n') = 0;
  write(server_socket, buffer, sizeof(buffer));
  
  /*
  while (1) {
    printf("enter data: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));
    printf("received: [%s]\n", buffer);
    }*/
}
