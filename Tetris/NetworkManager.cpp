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

bool NetworkManager::StartClient(const std::string& address, uint16_t port) {
    if (client != nullptr) {
        std::cerr << "A network host/client is already created.\n";
        return false;
    }

    // Cria um host para o cliente com 1 conexão de saída
    client = enet_host_create(nullptr, 1, 2, 0, 0);
    if (client == nullptr) {
        std::cerr << "An error occurred while trying to create an ENet client host.\n";
        return false;
    }

    ENetAddress enetAddress;
    enet_address_set_host(&enetAddress, address.c_str());
    enetAddress.port = port;

    // Tenta conectar ao servidor
    peer = enet_host_connect(client, &enetAddress, 2, 0);
    if (peer == nullptr) {
        std::cerr << "No available peers for initiating an ENet connection.\n";
        enet_host_destroy(client); // Limpa os recursos se falhar
        client = nullptr;
        return false;
    }

    // Aguarda a tentativa de conexão
    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connection to " << address << ":" << port << " succeeded.\n";
        return true;
    }
    else {
        enet_peer_reset(peer); // Limpa os recursos se falhar
        peer = nullptr;
        enet_host_destroy(client);
        client = nullptr;
        std::cerr << "Connection to " << address << ":" << port << " failed.\n";
        return false;
    }
}


bool NetworkManager::StartHost(uint16_t port) {
    if (client != nullptr) {
        std::cerr << "A network host/client is already created.\n";
        return false;
    }

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;

    // Cria um host que pode aceitar até 32 conexões de entrada
    client = enet_host_create(&address, 32, 2, 0, 0);
    if (client == nullptr) {
        std::cerr << "Failed to create an ENet server host.\n";
        return false;
    }

    std::cout << "Hosting a game on port " << port << std::endl;
    return true;
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

bool NetworkManager::IsConnected() {
    return (client != nullptr && peer != nullptr);
}

