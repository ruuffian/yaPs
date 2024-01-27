#include "include/server.h"

#define MAX_QUEUE_SIZE 15

int main(argc, argv)
int argc;
char** argv;
{
  int main_fd, port;
  
  switch(argc) 
  {
    case 1:
      port = DEFAULT_PORT;
      break;
    case 2:
      port = atoi(argv[1]);
      if(port == 0) {
        printf("[port] must be between 1024-9999\n");
        return 1;
      }
      break;
    default:
      printf("Usage: server [port]\n");
      break;
  }

  /* SHOULD REWRITE MAIN PORT AS UDP SOCKET !!!!!!!!! */
  main_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(main_fd == -1)
  {
    printf("[SERVER] [ERROR] Failed to open host socket\n");
    return 1;
  }
  
  printf("[SERVER] [INFO] Host socket successfully opened on fd [%d]\n", main_fd);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(DEFAULT_PORT);
  addr.sin_addr.s_addr = INADDR_ANY;

  if(bind(main_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
  {
    printf("[SERVER] [ERROR] Failed to bind host socket\n");
    return 1;
  }

  /*  MAIN PROGRAM LOOP
   *    Listen for requests on main endpoint and spawn chatrooms. 
   */
  while(listen(main_fd, MAX_QUEUE_SIZE) == 0) 
  { 
    struct chatroom* chatrooms;

    /* Open file descriptor for client connection */
    int client_fd = accept(main_fd, 0, 0);
    if(client_fd == -1)
    {
      printf("[SERVER] [NOTICE] Failed to accept client connection\n");
      continue;
    }
    printf("[SERVER] [INFO] Connected to client on descriptor [%d]\n", client_fd);

    ssize_t msg_size;    
    do 
    {
      char msg_buffer[MAX_MSG_SIZE];
      msg_size = recv(client_fd, msg_buffer, MAX_MSG_SIZE, 0); 
      if(msg_size == -1){
        printf("[SERVER] [NOTICE] Failed to recieve data from client\n");
        continue;
      } else if(msg_size == 0)
      {
        continue;
      }
      /* Create Chatroom, Join Chatroom, Leave Chatroom, Start DM, Leave DM */
      if(strncmp(msg_buffer, "CREATE", strlen("CREATE")) == 0)
      {
        switch(fork())
        {
          case -1:
            printf("[SERVER] [ERROR] Failed to fork child\n");
            continue;
          case 0: /* child */
            close(main_fd);
            struct pollfd* clients;
            spawn_chatroom(client_fd, &clients);
            chatrooms = calloc(1, sizeof(struct chatroom));
            chatrooms[0].clients = clients;
            chatrooms[0].nfds = 1;
            exit(host_chatroom(&clients));
            break;
          default: /* parent */
            close(client_fd);
            break;
        }
      } else if(strncmp(msg_buffer, "JOIN", strlen("JOIN")))
        {
          // Send "What chatroom would you like to join?
          int room_code = 0;
          struct chatroom room = chatrooms[room_code];

          join_chatroom(client_fd, room.clients, room.nfds);
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

/* Initialize a chatroom */
int spawn_chatroom(fd, clients) 
int fd;
struct pollfd** clients;
{
  struct pollfd* client_list = *clients;
  struct pollfd client;
  client.fd = fd;
  client.events = POLLIN;
  client_list = calloc(1, sizeof(struct pollfd));
  client_list[0] = client;

  printf("[SERVER] [INFO] Created chatroom on fd %d", fd);
  return 1;
}

int host_chatroom(clients)
struct pollfd** clients;
{
  int nfds = 1;
  struct pollfd* client_list = *clients;
  int fd = client_list[0].fd;

  /* Notify client of Chatroom opening */
  char welcome_msg[] = "Welcome to the Chatroom! Please do your best to be respectful.\n"; 
  ssize_t msg_size = send(fd, welcome_msg, strlen(welcome_msg) + 1, 0);
  if(msg_size == -1) {
    printf("[SERVER] [NOTICE] Failed to send response to client!\n");
  }

  while(1){}

  return 0;
}

int join_chatroom(fd, fds, nfds)
int fd;
struct pollfd* fds;
int nfds;
{
  struct pollfd new_client;
  new_client.fd = fd;
  new_client.events = POLLIN;
  realloc(fds, sizeof(struct pollfd) * ++nfds);
  fds[nfds - 1] = new_client;
  return nfds;
}
