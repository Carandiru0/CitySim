#pragma once

#include <iostream>

#include <enet/enet.h>
#include <SFML/Network.hpp>

class Net {
	public:
		Net(unsigned short port);
		~Net();

		void update();

	private:
		ENetHost *client;
		ENetAddress address;
		ENetEvent event;
		ENetPeer *peer;
};