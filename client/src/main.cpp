#include <iostream>
#include <enet/enet.h>
#define SECONDS(x) ((x)* 1000)

int main() {

  if (enet_initialize()) {
    fprintf(stderr, "An error occured while initializing ENet\n");
    return EXIT_FAILURE;
  };

  atexit(enet_deinitialize);

  ENetHost* client;
  client = enet_host_create(NULL, 1, 1, 0, 0);
  
  if (!client) {
    fprintf(stderr, "An error occured while trying to create an ENet client host\n");
    return EXIT_FAILURE;
  };

  ENetAddress address;
  ENetEvent event;
  ENetPeer* peer;

  enet_address_set_host(&address, "127.0.0.1");
  address.port = 7777;

  peer = enet_host_connect(client, &address, 1, 0);
  if (!peer) {
    fprintf(stderr, "No available peers for initializing an ENet connection\n");
    return EXIT_FAILURE;
  };

  if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
    puts("Connection to 127.0.0.1:7777 succeeded");
  }
  else {
    enet_peer_reset(peer);
    puts("Connection to 127.0.0.1:7777 failed");
  };

  while (enet_host_service(client, &event, SECONDS(1))) {
    switch(event.type) {
      case ENET_EVENT_TYPE_RECEIVE:
        printf("A packet of length %u containing %s was received from %x:%u on channel %u.\n",
          event.packet->dataLength,
          event.packet->data,
          event.peer->address.host,
          event.peer->address.port,
          event.channelID
        );
        break;
      default:
        break;
    };
  };

  enet_peer_disconnect(peer, 0);

  while (enet_host_service(client, &event, SECONDS(3)) > 0) {
    switch (event.type) {
      case ENET_EVENT_TYPE_RECEIVE:
        enet_packet_destroy(event.packet);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        puts("Disconnected successfully");
        break;
    };
  };

  return 0;
};