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
}

void Net::update() {
	int status = enet_host_service(client, &event, 100);

	if (status >= 0) {
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
}

Net::~Net() {
	enet_host_destroy(client);
	enet_deinitialize();
}