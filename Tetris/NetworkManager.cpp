#include "NetworkManager.h"
#include "GamePlayState.h"


NetworkManager::NetworkManager(std::shared_ptr <Context>& context) : m_context(context) {
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

void NetworkManager::sendPacket(ENetPeer* peer, MessageType type, const void* data, size_t dataLength) {
    ENetPacket* packet = enet_packet_create(data, dataLength, ENET_PACKET_FLAG_RELIABLE);
    std::cout << "PACOTE:" << packet << std::endl;
    std::cout << "A packet of length " << packet->dataLength << std::endl;
    std::cout << "PACOTE DATA: " << static_cast<int>(*reinterpret_cast<MessageType*>(packet->data)) << std::endl;
    MessageType* receivedType = reinterpret_cast<MessageType*>(packet->data);
    std::cout << "A packet of length " << packet->dataLength << " containing " << static_cast<int>(*receivedType) << std::endl;
    enet_peer_send(peer, 0, packet);
    enet_host_flush(client);
}

bool NetworkManager::StartClient(const std::string& address, uint16_t port) {
    isHost = false;
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
    std::cout << "CLIENT_CLIENT: " << client << std::endl;
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
        peer = event.peer;
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
    // Configura o host para ouvir apenas no localhost (127.0.0.1)
    enet_address_set_host(&address, "localhost");
    address.port = port;

    // Cria um host que pode aceitar até 32 conexões de entrada
    client = enet_host_create(&address, 1, 2, 0, 0);
    if (client == nullptr) {
        std::cerr << "Failed to create an ENet server host.\n";
        return false;
    }

    std::cout << "Hosting a game on port " << port << std::endl;
    isHost = true;
    std::cout << "CLIENT_SERVER: " << client << std::endl;
    return true;
}

void NetworkManager::ProcessNetworkEvents() {
    ENetEvent event;
    if (enet_host_service(client, &event, 0) > 0) {
        std::cout << "Processing network events...\n";
        if (client == nullptr) {
            std::cerr << "Client pointer is null.\n";
            return;
        }
        // Agora é seguro processar eventos de rede
        switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
        {
            ClientData* clientData = new ClientData();  // Cria uma nova instância de ClientData
            event.peer->data = clientData;
            break;  // O break agora está dentro do bloco de escopo
        }
        case ENET_EVENT_TYPE_RECEIVE:
            std::cout << "Chegou o pacote ae" << std::endl;
            std::cout << "PEER DATA: " << event.peer->data << std::endl;
            std::cout << "PEER: " << peer << std::endl;
            if (event.peer != nullptr){ //&& event.peer->data != nullptr) {
                std::cout << "A packet of length " << event.packet->dataLength
                    << " containing " << event.packet->data
                    << " was received from " << event.peer->data
                    << " on channel " << event.channelID << ".\n";
                // Clean up the packet now that we're done using it.
                ClientData* clientData = reinterpret_cast<ClientData*>(event.packet->data);
                MessageType messageType = clientData->start_flag;
                if (messageType == MessageType::GAME_READY) {
                    // Definir a flag para iniciar o jogo.
                    readyToStartGame = true;
                }
                enet_packet_destroy(event.packet);
            }
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            if (event.peer != nullptr) {
                std::cout << "Disconnection occurred.\n";
                if (event.peer->data != nullptr) {
                    // Cast de volta para o tipo correto e desalocar.
                    delete static_cast<ClientData*>(event.peer->data);
                    event.peer->data = nullptr;
                }
            }
            break;

        default:
            break;
        }
    }
}

// Isto deve ser chamado para notificar todos os clientes que o jogo está pronto.
void NetworkManager::notifyGameReady() {
    if (isHost) {
        MessageType messageType = MessageType::GAME_READY;

        for (size_t i = 0; i < client->peerCount; ++i) {
            ENetPeer* peer = &client->peers[i];

            // Você pode querer verificar se o peer foi atribuído um ID de cliente.
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                char clientIp[128];
                enet_address_get_host_ip(&peer->address, clientIp, sizeof(clientIp));
                std::cout << "CLIENTE " << clientIp << " RECEBERA A MENSAGEM DE ";

                char serverIp[128];
                enet_address_get_host_ip(&client->address, serverIp, sizeof(serverIp));
                std::cout << serverIp << std::endl;

                std::cout << "CLIENTE " << peer->address.host << " RECEBERA A MENSAGEM DE " << client->address.host << std::endl;
               // std::cout << "Enviando mensagem de jogo pronto para o cliente com ID: " << clientID << std::endl;
                sendPacket(peer, messageType, &messageType, sizeof(messageType));
            }
        }
        readyToStartGame = true;
    }
}

    //else {
    //    // Esta função não seria chamada pelo cliente no uso normal, pois o cliente
    //    // recebe a notificação do servidor. Você trataria isso no ProcessNetworkEvents.
    //}


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
    //std::cout << "client" << client << "           " << "peer" << peer << std::endl;
    if (isHost) {
        return true;
    }
    if (client == nullptr || peer == nullptr) {
        return false;
    }
    return true;
}

bool NetworkManager::isReadyToStartGame() {
    return readyToStartGame;
}

ENetPeer* NetworkManager::getClients() const {
    return client->peers;
}
