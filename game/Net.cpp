#include "Net.hpp"

using namespace std;

Net::Net(unsigned short port) {
	if (enet_initialize() != 0) {
		cerr << "An error occurred while initializing ENet.\n";
		exit(1);
	}

	client = enet_host_create(NULL, 1, 2, 0, 0);

	if (!client)
		cerr << "Error creating client\n";

	enet_address_set_host(&address, "localhost");
	address.port = 15817;

	peer = enet_host_connect(client, &address, 2, 0);

	if (peer == NULL) {
		cerr << "No available peers for initializing an ENet connection";
		exit(1);
	}

	int eventStatus = 1;

	while (1) {
		eventStatus = enet_host_service(client, &event, 1000);

		if (eventStatus >= 0) {
			switch (event.type) {
				case ENET_EVENT_TYPE_CONNECT:
					cout << "Connected to " << event.peer->address.host << "\n";
				
					break;

				case ENET_EVENT_TYPE_RECEIVE:
					printf("(Client) Message from server : %s\n", event.packet->data);
					enet_packet_destroy(event.packet);
					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					cout << "Disconnected from " << event.peer->address.host << "\n";
					event.peer->data = NULL;

					break;
			}
		}

		/*printf("Say > ");
		gets(message);

		if (strlen(message) > 0) {
			ENetPacket *packet = enet_packet_create(message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, 0, packet);
		}*/
	}
}

Net::~Net() {
	enet_host_destroy(client);
	enet_deinitialize();
}