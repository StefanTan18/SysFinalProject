#include "pipe_networking.h"
#include <signal.h>

//void process(char *s);
void subserver(int from_client);


/*static void sighandler(int signo) {
  if (signo == SIGINT) {
      remove("wkp");
      exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);

  int to_client;
  int from_client;
  int LIMIT = 10;
  int mafialimit = 2;
  int curnorm = 0;
  int curmafia = 0;
  int day_night = 0;
  
  
  while(1) {
    from_client = server_handshake( &to_client );
    
    
    
    while(curmafia && read(from_client, data, BUFFER_SIZE)) {
      printf("Server %d received: %s\n", getpid(), data);
      write(to_client, data, BUFFER_SIZE);
    }
  }
}
*/

int main() {

  int listen_socket;
  int f;
  listen_socket = server_setup();

  while (1) {

    int client_socket = server_connect(listen_socket);
    f = fork();
    if (f == 0)
      subserver(client_socket);
    else
      close(client_socket);
  }
}

void subserver(int client_socket) {
  //int LIMIT = 10;
  int mafialimit = 2;
  int curnorm = 0;
  int curmafia = 0;
  int day_night = 0;
  int num_players = 0;
  int curr_player = 0; 
  char buffer[BUFFER_SIZE];
  
  //Reads the number of players
  read(client_socket, buffer, sizeof(buffer));
  num_players = atoi(buffer);
  
       

  /*  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    process(buffer);
    write(client_socket, buffer, sizeof(buffer));
    }//end read loop */
  close(client_socket);
  exit(0);
}

/*
void process(char * s) {
  while (*s) {
    if (*s >= 'a' && *s <= 'z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    else  if (*s >= 'A' && *s <= 'Z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    s++;
  }
  }*/
