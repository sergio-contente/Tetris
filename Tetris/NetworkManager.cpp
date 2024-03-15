#include "NetworkManager.h"

NetworkManager::NetworkManager() {
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet.\n";
    }
}

NetworkManager::~NetworkManager() {
    if (client != nullptr) {
        enet_host_destroy(client);
    }
    enet_deinitialize();
}

bool NetworkManager::StartConnection(const std::string& address, uint16_t port) {
    client = enet_host_create(nullptr /* create a client host */,
        1 /* only allow 1 outgoing connection */,
        2 /* allow up 2 channels to be used, 0 and 1 */,
        0 /* assume any amount of incoming bandwidth */,
        0 /* assume any amount of outgoing bandwidth */);
    if (client == nullptr) {
        std::cerr << "An error occurred while trying to create an ENet client host.\n";
        return false;
    }

    ENetAddress enetAddress;
    enet_address_set_host(&enetAddress, address.c_str());
    enetAddress.port = port;

    peer = enet_host_connect(client, &enetAddress, 2, 0);
    if (peer == nullptr) {
        std::cerr << "No available peers for initiating an ENet connection.\n";
        return false;
    }

    // Wait up to 5 seconds for the connection attempt to succeed.
    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connection to " << address << ":" << port << " succeeded.\n";
        return true;
    }
    else {
        enet_peer_reset(peer);
        std::cerr << "Connection to " << address << ":" << port << " failed.\n";
        return false;
    }
}

void NetworkManager::ProcessNetworkEvents() {
    ENetEvent event;
    while (enet_host_service(client, &event, 0) > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_RECEIVE:
            std::cout << "A packet of length " << event.packet->dataLength
                << " containing " << event.packet->data
                << " was received from " << event.peer->data
                << " on channel " << event.channelID << ".\n";
            // Clean up the packet now that we're done using it.
            enet_packet_destroy(event.packet);
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            std::cout << "Disconnection occurred.\n";
            event.peer->data = nullptr;
            break;

            // Handle other events...
        }
    }
}

void NetworkManager::Disconnect() {
    if (peer != nullptr) {
        enet_peer_disconnect(peer, 0);
        // Allow some time for the disconnect to be acknowledged
        ENetEvent event;
        while (enet_host_service(client, &event, 3000) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Disconnection succeeded.\n";
                return;
            }
        }
        // If the disconnection wasn't acknowledged, reset the peer
        enet_peer_reset(peer);
        peer = nullptr;
    }
}
