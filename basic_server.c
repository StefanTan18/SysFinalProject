#include "pipe_networking.h"

//void process(char *s);
void subserver(int from_client);

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
  int civil_left = 0;
  int mafia_left = 0;
  int day_night = 0;
  int num_players = 0;
  char buffer[BUFFER_SIZE];
  
  //Reads the number of players
  read(client_socket, buffer, sizeof(buffer));
  num_players = atoi(buffer);

  //Assigning roles
  strcpy(buffer, "The Server is now assigning roles...");
  write(client_socket, buffer, sizeof(buffer));
  
  int mafia0 = rand() % num_players;
  int mafia1 = rand() % num_players;
  int police = rand() % num_players;
  while (mafia0 == mafia1) {
    mafia1 = rand() % num_players;
  }
  while (police == mafia0 && police == mafia1) {
    police = rand() % num_players;
  }

  mafia_left = 2;
  civil_left = num_players - mafia_left;

  int i = 0; 
  while(read(client_socket, buffer, sizeof(buffer)) && i < num_players) {
    if (i == mafia0) {
      char other_mafia[2];
      sprintf(other_mafia, "%d", mafia1);
      strcpy(buffer, "You are a member of the mafia.\n Goal: Work together to eliminate all non-mafia players before they find out!\n Other Mafia Member: ");
      strcat(buffer, other_mafia);
      write(client_socket, buffer, sizeof(buffer));
      i++;
    }
    if (i == mafia1) {
      char other_mafia[2];
      sprintf(other_mafia, "%d", mafia0);
      strcpy(buffer, "You are a member of the mafia.\n Goal: Work together to eliminate all non-mafia players before they find out!\n Other Mafia Member: ");
      strcat(role, other_mafia);
      write(client_socket, buffer, sizeof(buffer));
      i++;
    }
    if (i == police) {
      strcpy(buffer, "You are the police.\n Goal: Find and arrest all the member of the mafia before it's too late!");
      write(client_socket, buffer, sizeof(buffer));
      i++;
    }
    else {
      strcpy(buffer, "You are a civilian.\n Goal: Work together with other civilians to get rid of the mafia members hiding within your community!");
      write(client_socket, buffer, sizeof(buffer));
      i++;
    }
  }
      
  
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
