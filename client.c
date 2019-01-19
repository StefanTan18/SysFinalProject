#include "networking.h"

//socket variables
int server_socket;

//Determines which player was chosen in the vote
int getMajority(int vote[], int size) {
  int current = 0;
  for(int i = 0; i < size;i++){
    if(vote[i] > vote[current]){
      current = i;
    }
  }
  return current;
}

//Determines if vote is valid
int valid_kill(int notalive[], int numDead, int recently_killed) {

    int yes = 0;
    for(int i = 0; i < numDead; i++) {
        if(recently_killed == notalive[i]) yes = 1;
    }
    return yes;

}

int main(int argc, char **argv) {

  //sighandler
  //signal(SIGINT, sighandler);

  //Initializing variables
  char buffer[BUFFER_SIZE];
  int num_players;

  //Socket setup
  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  //Prompting user to enter the number of players
  printf("Enter the number of players(6-10 players Only!): \n");
  fgets(buffer, sizeof(buffer), stdin);
  num_players = atoi(buffer);

  //Checks input for correctness
  while(num_players < 6 || num_players > 10) {
    printf("Input invalid.  Please enter a correct value (6-10): ");
    fgets(buffer, sizeof(buffer), stdin);
    num_players = atoi(buffer);
  }

  //Writing num_players to socket
  *strchr(buffer, '\n') = 0;
  write(server_socket, buffer, sizeof(buffer));

  //Initializing game variables
  int notalive[num_players];//if players are alive
  int numDead = 0; //number of dead players
  int recently_killed = 0; //stores the most recent death
  int votes[num_players]; //store the votes
  int your_choice = 0; //placed into vote

  //Initializing votes array
  for (int i = 0; i < num_players; i++) {
    votes[i] = 0;
  }

  //Receiving roles
  read(server_socket, buffer, sizeof(buffer));
  printf("%s\n", buffer);

  //Communicating information to players
  for (int i = 0; i < num_players; i++) {

    //Welcoming and confirming identities of players
    printf("Welcome Player_%d!\n", i);
    printf("To confirm that you are Player_%d, please enter 'y': \n", i);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[0] = tolower(buffer[0]);

    //Error checking the input
    while(strcmp(buffer, "y\n")) {
      printf("Waiting for player to confirm identity with 'y': ");
      fgets(buffer, sizeof(buffer), stdin);
      buffer[0] = tolower(buffer[0]);
    }

    //Getting roles from server
    *strchr(buffer, '\n') = 0;
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));

    printf("\nCONFIDENTIAL INFO!\n\n %s\n\n", buffer);
    printf("Enter 'y' if understood: \n");

    //Confirming confidentiality
    fgets(buffer, sizeof(buffer), stdin);

    //Error checking input
    while(strcmp(buffer, "y\n")) {
      printf("Waiting for player to signal 'y': ");
      fgets(buffer, sizeof(buffer), stdin);
      buffer[0] = tolower(buffer[0]);
    }

    //Making terminal look nice
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("Switch to the next player.\n");
  }

  //Starting the game
  //Waiting for confirmation
  printf("Enter 'y' if you are ready to start mafia.\n ");
  fgets(buffer, sizeof(buffer), stdin);
  buffer[0] = tolower(buffer[0]);

  //Error checking input
  while(strcmp(buffer, "y\n")) {
    printf("Waiting for player to signal 'y': \n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[0] = tolower(buffer[0]);
  }


  int i = 0;
  while(i < num_players) {
    printf("Welcome Player_%d!\n", i);
    printf("To confirm that you are Player_%d, please enter 'y':\n", i);
    fgets(buffer,sizeof(buffer), stdin);
    while(strcmp(buffer, "y\n")) {
      printf("Waiting for player to confirm identity with 'y': \n");
      fgets(buffer, sizeof(buffer), stdin);
      buffer[0] = tolower(buffer[0]);
    }
    sprintf(buffer, "%d\n", i);
    *strchr(buffer, '\n') = 0;
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    //printf("%d\n",strncmp(buffer, "G", 1));

    if (!strcmp(buffer, "It's nighttime in the community. Time to eliminate a civilian!\n")) {
      printf("Select your target (0-%d)\n: ", num_players - 1);
      fgets(buffer, sizeof(buffer), stdin);
      recently_killed = atoi(buffer);
      while(buffer[0] >= 'A' || recently_killed >= num_players || recently_killed < 0 || valid_kill(notalive, numDead, recently_killed)) {//valid_kill returns 0 if vote is valid
        printf("Invalid Input. Please select a valid player:\n");
        fgets(buffer, sizeof(buffer), stdin);
        recently_killed = atoi(buffer);
      }

      notalive[numDead] = recently_killed;
      numDead++;
      *strchr(buffer, '\n') = 0;
      write(server_socket, buffer, sizeof(buffer));
      if (numDead == num_players - 2) {
        break;
      }
    }
    else if (!strncmp(buffer, "G",1)) {
      printf("Let the vote commence! Who do you believe is the mafia member? (0-%d)\n", num_players - 1);
      fgets(buffer, sizeof(buffer), stdin);
      your_choice = atoi(buffer);
      votes[your_choice]++;
      while(buffer[0] >= 'A' || your_choice >= num_players || your_choice < 0 || valid_kill(notalive, numDead, your_choice)) {//valid_kill returns 0 if vote is valid
        printf("Invalid Selection. Please choose a valid player to vote against:\n");
        fgets(buffer, sizeof(buffer), stdin);
        your_choice = atoi(buffer);
      }
      votes[your_choice] += 1;


        if (i == num_players - 1) {
          printf("Are you sure? Enter 'y' to confirm: \n");
          fgets(buffer, sizeof(buffer), stdin);
          buffer[0] = tolower(buffer[0]);
          while(strcmp(buffer, "y\n")) {
            printf("Waiting for player to indicate they are finished with 'y': ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[0] = tolower(buffer[0]);
          }
              printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
              printf("All players can see this!\n");
              printf("The vote has concluded.\n");
              int maj = getMajority(votes,num_players);
              printf("The majority has decided that Player_%d should be executed.\n", maj);// this should happen after looping all players
              printf("Player_%d has been executed.\n", maj);//segfault here
              sprintf(buffer, "%d\n",maj);
              *strchr(buffer, '\n') = 0;
              write(server_socket, buffer, sizeof(buffer));
              read(server_socket, buffer, sizeof(buffer));
              printf("%s\n", buffer);
              if (!strcmp(buffer, "THE MAFIA HAS BEEN EXECUTED!\n")) {
                break;
              }
      }
      else{
        printf("Are you sure? Enter 'y' to confirm: \n");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[0] = tolower(buffer[0]);
        while(strcmp(buffer, "y\n")) {
          printf("Waiting for player to indicate they are finished with 'y': ");
          fgets(buffer, sizeof(buffer), stdin);
          buffer[0] = tolower(buffer[0]);
        }
        write(server_socket, buffer, sizeof(buffer));

      }
}

    printf("Enter 'y' if done: \n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[0] = tolower(buffer[0]);
    while(strcmp(buffer, "y\n")) {
      printf("Waiting for player to signal they are finished with 'y': \n");
      fgets(buffer, sizeof(buffer), stdin);
      buffer[0] = tolower(buffer[0]);
    }
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("Switch to the next player.\n");
    i++;
    if (i == num_players) {
      i = 0;
      for (int j = 0; j < num_players; j++) {
        votes[j] = 0;
      }
    }
  }


  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  write(server_socket,buffer,sizeof(buffer));
  read(server_socket, buffer, sizeof(buffer));
  printf("%s\n", buffer);
  printf("\nTHANKS FOR PLAYING!\n");
}

/*void sighandler() {

  //close sockets
  close(server_socket);

  exit(0);
}
*/
