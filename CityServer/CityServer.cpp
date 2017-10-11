#include <iostream>

#include <enet/enet.h>

using namespace std;

int main(int argc, char *argv[]) {
	if (enet_initialize() != 0) {
		cerr << "An error occurred while initializing ENet.\n";
		return EXIT_FAILURE;
	}

	ENetAddress address;
	ENetHost *server;

	enet_address_set_host(&address, "localhost");
	address.port = 15817;

	server = enet_host_create(&address, 10, 2, 0, 0);

	if (server == NULL) {
		cerr << "Cannot create server!";
		return EXIT_FAILURE;
	}
	else
		cout << "Running server...\n";

	ENetEvent event;

	while (enet_host_service(server, &event, 2000) >= 0) {
		switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				cout << event.peer->address.host;
				cout << " connected\n";

				break;

			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				cout << event.peer->address.host;
				cout << " disconnected\n";
				event.peer->data = NULL;
		}
	}

	enet_host_destroy(server);
	enet_deinitialize();
}