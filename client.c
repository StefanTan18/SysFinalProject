#include "pipe_networking.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  //Setting up the Game
  int num_players = 0;
  
  printf("Enter the number of players(5-10 players Only!): ");
  fgets(buffer, sizeof(buffer), stdin);
  *strchr(buffer, '\n') = 0;
  num_players = atoi(buffer);
  write(server_socket, buffer, sizeof(buffer));
  
  //Receiving roles
  read(server_socket, buffer, sizeof(buffer));
  printf("%s\n", buffer);

  for (int i = 0; i < num_players; i++) {
    printf("Welcome Player_%d!\n", i);
    printf("To confirm that you are Player_%d, please enter 'y': ", i);
    fgets(buffer, sizeof(buffer), stdin);
    if (!strcmp(buffer, "y\n")) {
      write(server_socket, buffer, sizeof(buffer));
      read(server_socket, buffer, sizeof(buffer));
      printf("\nCONFIDENTIAL INFO!\n\n %s\n\n", buffer);
      printf("Enter 'y' if understood: ");
      fgets(buffer, sizeof(buffer), stdin);
      if (!strcmp(buffer, "y\n")) {
	execlp("clear", "clear", NULL);
	printf("Switch to the next player.");
      }
    }
  }
      
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
