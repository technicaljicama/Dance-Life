#include <SDL2/SDL_net.h>

extern void (*net_func)();

void net_create_client(char* host);
void net_create_server();
void net_read_keys();
void net_send_keys();
void net_server_send_keys();
void net_client_read_keys();
