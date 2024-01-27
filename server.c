#include "include/server.h"


#define MAX_QUEUE_SIZE 15
#define MAX_MSG_SIZE 500

int main(void) {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_fd == -1){
    printf("[SERVER] [ERROR] Failed to open host socket\n");
    return 1;
  }
  
  printf("[SERVER] [INFO] Host socket successfully opened on fd [%d]\n", socket_fd);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(3000);
  addr.sin_addr.s_addr = INADDR_ANY;

  if(bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1){
    printf("[SERVER] [ERROR] Failed to bind host socket\n");
    return 1;
  }

  while(listen(socket_fd, MAX_QUEUE_SIZE) == 0) { 
    int client_fd = accept(socket_fd, 0, 0);
    if(client_fd == -1){
      printf("[SERVER] [NOTICE] Failed to accept client connection\n");
      continue;
    }

    printf("[SERVER] [INFO] Connection recieved, assigned fd [%d]\n", client_fd);
    ssize_t msg_size;    
    do {
      char msg_buffer[MAX_MSG_SIZE];
      msg_size = recv(client_fd, msg_buffer, MAX_MSG_SIZE, 0); 
      if(msg_size == -1){
        printf("[SERVER] [NOTICE] Failed to recieve data from client\n");
        continue;
      } else if(msg_size == 0){
        continue;
      }

      printf("[SERVER] [INFO] Recieved message on fd [%d]: %s\n", client_fd, msg_buffer);
      msg_size = send(client_fd, msg_buffer, MAX_MSG_SIZE, 0);
      if(msg_size == -1) {
        printf("[SERVER] [NOTICE] Failed to send response to client!\n");
        continue;
      }
      printf("[SERVER] [INFO] Successfully echoed client payload\n");
    } while(msg_size > 0);

    close(client_fd);
    printf("[SERVER] [INFO] Connection on fd [%d] closed\n", client_fd);
  }
  
  return 0;
}
