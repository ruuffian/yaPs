#include "include/client.h"

#define MAX_MSG_SIZE 500

/*
 * main - yaPs chat client
 *
 * Intended features:
 *   [x] Connect to yaPs server
 *   [] Create chatroom
 *   [] Send/recieve messages in chatroom
 *   [] Message history
 *   [] Create private chatrooms
 *   [] Open multiple chatrooms at once
 *   [] Create user
 *   [] Log in and out
 */

int main(int argc, char** argv) {
  int fd, port;

  switch(argc) {
    case 1:
      port = DEFAULT_PORT;
      break;
    case 2:
      port = atoi(argv[1]);
      if(port > 0 && port < 1024){
        printf("[port] must be between 1024-9999\n");
        return 1;
      }
      break;
    default:
      printf("Usage: client [port]\n");
      return 1;
  }

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if(fd == -1){ 
    printf("Failed to open socket file descriptor\n");
    return 1;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;

  if(connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) return 1; 
  printf("[CLIENT] [INFO] Successfully connected to server\n");

  while(1){
    printf("[CLIENT] Input message:: ");
    char msg[MAX_MSG_SIZE];
    fgets(msg, MAX_MSG_SIZE, stdin);
    msg[strcspn(msg, "\n")] = 0;
    ssize_t msg_size = send(fd, msg, strlen(msg) + 1, 0); 
    msg[0] = '\0';

    if(msg_size == -1) return 1;
    printf("[CLIENT] [INFO] Message sent successfully!\n");

    char res_buffer[MAX_MSG_SIZE];
    ssize_t response_size = recv(fd, res_buffer, MAX_MSG_SIZE, 0);
    if(response_size == -1) return 1;
    printf("[CLIENT] [INFO] Recieved response from server: %s\n", res_buffer);
    res_buffer[0] = '\0';
  }

  close(fd);
  printf("[CLIENT] [INFO] Connection terminated\n");

  return 0;
}
