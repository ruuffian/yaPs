#include "include/client.h"

#define MAX_MSG_SIZE 500

int main(void) {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_fd == -1) return 1;

  struct sockaddr addr = {
    AF_INET,
    htons(3000),
    INADDR_ANY
  };

  if(connect(socket_fd, &addr, sizeof(addr)) == -1) return 1; 
  printf("[INFO] Successfully connected to server\n");

  for(int i = 0; i < 5; ++i){
    char msg[] = "Hello, World!"; 
    ssize_t msg_size = send(socket_fd, msg, strlen(msg) + 1, 0); 

    if(msg_size == -1) return 1;
    printf("[INFO] Message sent successfully!\n");
  }

  close(socket_fd);
  printf("[INFO] Connection terminated\n");

  return 0;
}
