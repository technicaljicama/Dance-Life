#include "net.h"
#include "engine.h"

#include <SDL2/SDL_net.h>
#include <stdbool.h>

static TCPsocket server, client, sock;

void (*net_func)();

void net_create_server() {
    SDLNet_Init();
    bool done = false;
    
    
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, NULL, 8864) == -1) {
        printf("ResolveHost: %s\n", SDLNet_GetError());
    }
    server = SDLNet_TCP_Open(&ip);
    if (!server) {
        printf("open: %s\n", SDLNet_GetError());
    }
    
    while (!done) {
        client = SDLNet_TCP_Accept(server);
        if (!client) {
            SDL_Delay(100);
            continue;
        }
        
        IPaddress *remoteip;
        remoteip = SDLNet_TCP_GetPeerAddress(client);
        
        printf("Accepted a connection\n");
        done = true;
        net_func = net_read_keys;
//         while (1) {
//             // char message[1024]
//                 
// 
//         }
        // SDLNet_TCP_Close(client);
    }
}

void net_create_client(char* host) {
    SDLNet_Init();
    
    IPaddress ip;
    
    if (SDLNet_ResolveHost(&ip, host, 8864) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(1);
    }
    
    sock = SDLNet_TCP_Open(&ip);
    if (!sock) {
        printf("open: %s\n", SDLNet_GetError());
    }

    net_func = net_send_keys;
}

void net_read_keys() {
    SDLNet_TCP_Recv(client, other_keys, sizeof(other_keys));
}

void net_send_keys() {
    SDLNet_TCP_Send(sock, keys, sizeof(keys));
}

void net_server_send_keys() {
    SDLNet_TCP_Send(client, keys, sizeof(keys));
}

void net_client_read_keys() {
    SDLNet_TCP_Recv(sock, other_keys, sizeof(other_keys));
}
