#include "networking.h"

//void process(char *s);
void subserver(int from_client);

int main() {

  int listen_socket;
  int f;
  listen_socket = server_setup();
 
  while (1) {

    int client_socket = server_connect(listen_socket);
    f = fork();
    if (f == 0) subserver(client_socket);
    else close(client_socket);
  } 
}

void subserver(int client_socket) {
  
  //incrementing shmem
  /* int key = 99;
  int shmid = shmget(key, sizeof(int), NULL);
  int *num =(int *) shmat(shmid, NULL, 0);
  *num = (*num) + 1;
  shmdt(num);    
  */
  
  //int LIMIT = 10;
  int civil_left = 0;
  int mafia_left = 0;
  int curr_player = 0;
  int curr_day = 0; //day number
  int day_night = 0; //night = 0, day = 1
  int num_players = 0; //total number of players
  int recently_killed = 0; //stores the most recent death
  char buffer[BUFFER_SIZE];

  //Reads the number of players
  read(client_socket, buffer, sizeof(buffer));
  num_players = atoi(buffer);

  //Assigning roles
  strcpy(buffer, "The Server is now assigning roles...");
  write(client_socket, buffer, sizeof(buffer));

  int mafia = rand() % num_players;

  mafia_left = 1;
  civil_left = num_players - mafia_left;

  int i = 0;
  while(read(client_socket, buffer, sizeof(buffer)) && i < num_players) {
    if (i == mafia) {
      strcpy(buffer, "You are a member of the mafia.\nGoal: Eliminate everyone else before they find out!\n");
      write(client_socket, buffer, sizeof(buffer));
      i++;
    }
    else {
      strcpy(buffer, "You are a civilian.\nGoal: Work together with other civilians to get rid of the 1 mafia member hiding within your community!");
      write(client_socket, buffer, sizeof(buffer));
      i++;
    }
  }

  //Game
  while (mafia_left && civil_left != 1) {
    read(client_socket, buffer, sizeof(buffer));
    curr_player = atoi(buffer);
    if (!day_night) {
      if (curr_player == mafia){
        strcpy(buffer, "It's nighttime in the community. Time to eliminate a civilian!");
        write(client_socket, buffer, sizeof(buffer));
        read(client_socket, buffer, sizeof(buffer));
        recently_killed = atoi(buffer);
        civil_left--;
      }
      else {
        strcpy(buffer, "It's nighttime in the community and you are currently sleeping.");
          write(client_socket, buffer, sizeof(buffer));
      }
      day_night++;
    }
    else if (day_night == 1) {
      curr_day++;
      sprintf(buffer, "It's day number %d in our beautiful community. Unfortunately Player_%d has been killed by the mafia.\n", curr_day, recently_killed);
      write(client_socket, buffer, sizeof(buffer));
      read(client_socket, buffer, sizeof(buffer));
      if (atoi(buffer) == mafia) {
        mafia_left--;
        strcpy(buffer, "THE MAFIA HAS BEEN EXECUTED!");
        write(client_socket, buffer, sizeof(buffer));
      }
      else {
        civil_left--;
        strcpy(buffer, "AN INNOCENT CIVILIAN HAS BEEN EXECUTED!");
        write(client_socket, buffer, sizeof(buffer));
      }
    }
  }

  if (mafia_left == 0) {
    strcpy(buffer, "Justice has prevailed! The mafia member has been executed and peace has returned to our community. GOOD WORK!");
    write(client_socket, buffer, sizeof(buffer));
  }
  else if (civil_left == 1) {
    strcpy(buffer, "One civilian left alone against one mafia member. That is not going to end up well. WELL DONE TO THE MAFIA!");
    write(client_socket, buffer, sizeof(buffer));
  }

  /*  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    process(buffer);
    write(client_socket, buffer, sizeof(buffer));
    }//end read loop */
  close(client_socket);
  exit(0);
}
