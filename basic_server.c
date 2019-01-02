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

  while(1) {
    from_client = server_handshake( &to_client );
    char data[BUFFER_SIZE];

    while(read(from_client, data, BUFFER_SIZE)) {
      printf("Subserver %d received: %s\n", getpid(), data);
      strcat(data, " *hiccup");
      write(to_client, data, BUFFER_SIZE);
    }
  }
}
