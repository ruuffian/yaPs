#include "include/client.h"

#define MAX_MSG_SIZE 500

int main(void) {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_fd == -1) return 1;

  struct sockaddr_in addr;
   addr.sin_family = AF_INET;
   addr.sin_port = htons(3000);
   addr.sin_addr.s_addr = INADDR_ANY;

  if(connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) return 1; 
  printf("[CLIENT] [INFO] Successfully connected to server\n");

  while(1){
    printf("[CLIENT] Input message:: ");
    char msg[MAX_MSG_SIZE];
    fgets(msg, MAX_MSG_SIZE, stdin);
    msg[strcspn(msg, "\n")] = 0;
    ssize_t msg_size = send(socket_fd, msg, strlen(msg) + 1, 0); 
    msg[0] = '\0';

    if(msg_size == -1) return 1;
    printf("[CLIENT] [INFO] Message sent successfully!\n");

    char res_buffer[MAX_MSG_SIZE];
    ssize_t response_size = recv(socket_fd, res_buffer, MAX_MSG_SIZE, 0);
    if(response_size == -1) return 1;
    printf("[CLIENT] [INFO] Recieved response from server: %s\n", res_buffer);
    res_buffer[0] = '\0';
  }

  close(socket_fd);
  printf("[CLIENT] [INFO] Connection terminated\n");

  return 0;
}
