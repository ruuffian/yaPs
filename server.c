#include "include/server.h"

#define MAX_QUEUE_SIZE 15
#define MAX_MSG_SIZE 500
int main(void) {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_fd == -1) return 1;
  
  printf("[SERVER] Host socket successfully opened on fd [%d]\n", socket_fd);

  struct sockaddr addr = {
    AF_INET,
    htons(3000),
    INADDR_ANY
  };

  if(bind(socket_fd, &addr, sizeof(addr)) == -1) return 1;

  while(listen(socket_fd, MAX_QUEUE_SIZE) == 0) { 
    int client_fd = accept(socket_fd, 0, 0);
    if(client_fd == -1) return 1;
    printf("[SERVER] Connection recieved, assigned fd [%d]\n", client_fd);
    ssize_t msg_size;    
    do {
      char msg_buffer[MAX_MSG_SIZE];
      msg_size = recv(client_fd, msg_buffer, MAX_MSG_SIZE, 0); 
      if(msg_size == -1) return 1;

      printf("[SERVER] Recieved message on fd [%d]: %s\n", client_fd, msg_buffer);
    } while(msg_size > 0);
    close(client_fd);
    printf("[SERVER] Connection on fd [%d] closed\n", client_fd);
  }
  
  return 0;
}
