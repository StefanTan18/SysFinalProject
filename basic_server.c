#include "pipe_networking.h"
#include <signal.h>

static void sighandler(int signo) {
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
