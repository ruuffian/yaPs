#include "include/server.h"

#define MAX_QUEUE_SIZE 15

int main(int argc, char** argv) {
  int main_fd, port;

  switch(argc) {
    case 1:
      port = DEFAULT_PORT;
      break;
    case 2:
      port = atoi(argv[1]);
      if(port == 0) {
        printf("[port] must be between 1024-9999\n");
        return 1;
      }
    default:
      printf("Usage: server [port]\n");
      break;
  }

  main_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(main_fd == -1){
    printf("[SERVER] [ERROR] Failed to open host socket\n");
    return 1;
  }
  
  printf("[SERVER] [INFO] Host socket successfully opened on fd [%d]\n", main_fd);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(DEFAULT_PORT);
  addr.sin_addr.s_addr = INADDR_ANY;

  if(bind(main_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1){
    printf("[SERVER] [ERROR] Failed to bind host socket\n");
    return 1;
  }

  /*  MAIN PROGRAM LOOP
   *    Listen for requests on main endpoint and spawn chatrooms. 
   */
  while(listen(main_fd, MAX_QUEUE_SIZE) == 0) { 

    /* Open file descriptor for client connection */
    int client_fd = accept(main_fd, 0, 0);
    if(client_fd == -1){
      printf("[SERVER] [NOTICE] Failed to accept client connection\n");
      continue;
    }
    printf("[SERVER] [INFO] Connected to client on descriptor [%d]\n", client_fd);

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
      switch(fork()){
        case -1:
          printf("[SERVER] [ERROR] Failed to fork child\n");
          continue;
        case 0: /* child */
          close(main_fd);
          exit(spawn_chatroom(client_fd));
          break;
        default: /* parent */
          close(client_fd);
          break;
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
    printf("\n[SERVER] [INFO] Connection on fd [%d] closed\n", client_fd);
  }
  
  return 0;
}

int spawn_chatroom(fd) 
int fd;
{
  printf("[SERVER] [INFO] Created chatroom on fd %d", fd);
  char welcome_msg[] = "Welcome to the ChatRoom! Please do your best to be respectful.\n"; 
  ssize_t msg_size = send(fd, welcome_msg, strlen(welcome_msg) + 1, 0);
  if(msg_size == -1) {
    printf("[SERVER] [NOTICE] Failed to send response to client!\n");
    return 1;
  }
  printf("[SERVER] [INFO] Successfully echoed client payload\n");
  return 0;
}
