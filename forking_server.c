#include "networking.h"

void subserver(int from_client);

//Controls functionality server-side
int main() {

  //Setting up socket variables
  int listen_socket;
  int f;
  listen_socket = server_setup();

  //Connecting users to server
  while (1) {

    int client_socket = server_connect(listen_socket);
    f = fork();
    if (f == 0) subserver(client_socket);
    else close(client_socket);
  }
}

//Function to help control functionality
void subserver(int client_socket) {

  //Initializing game variables
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

  //Setting all players' status to alive
  int lifestatus[num_players];
  for(int i =0; i < num_players;i++){
    lifestatus[i] = 0;
  }

  //Assigning roles
  strcpy(buffer, "The Server is now assigning roles...\n");
  write(client_socket, buffer, sizeof(buffer));

  //Randomly choosing the mafia member
  int mafia = rand() % num_players;

  //Setting the appropriate number of mafia and civilians left
  mafia_left = 1;
  civil_left = num_players - mafia_left;

  //Telling the users their role
  int i = 0;
  while(i < num_players) {
    if (i == mafia) {
      read(client_socket, buffer, sizeof(buffer));
      strcpy(buffer, "You are a member of the mafia.\nGoal: Eliminate everyone else before they find out!\n");
      write(client_socket, buffer, sizeof(buffer));
      i++;
    }
    else {
      read(client_socket, buffer, sizeof(buffer));
      strcpy(buffer, "You are a civilian.\nGoal: Work together with other civilians to get rid of the 1 mafia member hiding within your community!\n");
      write(client_socket, buffer, sizeof(buffer));
      i++;
    }
  }

  //Game
  //Loops until conditions are met for a team to win
  while (mafia_left && civil_left > 1) {

    //Reads the current player from the socket
    read(client_socket, buffer, sizeof(buffer));
    curr_player = atoi(buffer);

    //Controls gameplay at night
    if (!day_night) {

      //Uses current player to control day/night cycle
      if(curr_player == num_players - 1) day_night++;

      //Functionality to allow the mafia to choose their target
      if (curr_player == mafia){
        strcpy(buffer, "It's nighttime in the community. Time to eliminate a civilian!\n");
        write(client_socket, buffer, sizeof(buffer));
        read(client_socket, buffer, sizeof(buffer));
        recently_killed = atoi(buffer);
	      lifestatus[recently_killed] = 1;
        civil_left--;
      }

      //Functionality for civilians at night (they don't do anything)
      else {
        strcpy(buffer, "It's nighttime in the community and you are currently sleeping.\n");
        write(client_socket, buffer, sizeof(buffer));
      }
    }

    //Increments the day number counter
    else if (day_night) curr_day++;

    //Changes the day night counter back to night
    if(curr_player == num_players - 1) day_night--;

    //Informing chosen player of their death
    if(lifestatus[curr_player]){
	    strcpy(buffer, "Waking up in the middle of the night, you see a face. Swiftly darkness wrapped around you as you wish to see your mother one last time. YOU DIED! Or maybe that was a memory from a previous turn so stop hogging the keyboard\n");
	    write(client_socket, buffer, sizeof(buffer));
    }

    //Informing players of who was chosen to die
    else if(curr_player != num_players -1){
	    sprintf(buffer, "Good morning! It's day number %d in our beautiful community. Unfortunately Player_%d has been killed by the mafia.\n", curr_day, recently_killed);
	    write(client_socket, buffer, sizeof(buffer));
	    read(client_socket, buffer, sizeof(buffer));
    }

    //Voting functionality for the day
    else{
	    write(client_socket, buffer, sizeof(buffer));
	    read(client_socket, buffer, sizeof(buffer));

      //Executing the mafia if voted as such
	    if (atoi(buffer) == mafia) {
	      mafia_left--;
	      lifestatus[atoi(buffer)] = 1;
	      strcpy(buffer, "THE MAFIA HAS BEEN EXECUTED!\n");
	      write(client_socket, buffer, sizeof(buffer));
	    }

      //Executing a civilian if voted as such
	    else {
	      lifestatus[atoi(buffer)] = 1;
	      civil_left--;
	      strcpy(buffer, "AN INNOCENT CIVILIAN HAS BEEN EXECUTED!\n");
	      write(client_socket, buffer, sizeof(buffer));
	    }
    }



    }
  }

  //Victory conditions
  //Checking for civilian victory
  if (mafia_left == 0) {
    strcpy(buffer, "Justice has prevailed! The mafia member has been executed and peace has returned to our community. GOOD WORK!\n");
    write(client_socket, buffer, sizeof(buffer));
  }

  //Checking for mafia victory
  else if (civil_left == 1) {
    strcpy(buffer, "One civilian left alone against one mafia member. That is not going to end up well. WELL DONE TO THE MAFIA!\n");
    write(client_socket, buffer, sizeof(buffer));
  }

  //Checking for tie
  else {
    strcpy(buffer, "It seems we have a tie.  That means you broke the game.  Well done?\n");
    write(client_socket, buffer, sizeof(buffer));
  }

  //Terminating program
  close(client_socket);
  exit(0);
}
