#include "commons.h"
#include <poll.h>

int spawn_chatroom(int fd, struct pollfd** clients);
int host_chatroom(struct pollfd** client);
int join_chatroom(int fd, struct pollfd* fds, int nfds);

struct chatroom {
  struct pollfd* clients;
  int nfds;
};
