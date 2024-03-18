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
    //if (client == nullptr) {
    //    // Tratamento de erro: client não inicializado
    //    return;
    //}
    std::cout << "Hora de enviar um pacotinho hehehe" << std::endl;
    ENetPacket* packet = enet_packet_create(data, dataLength, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(client); // Imediatamente tenta enviar o pacote.
}

void NetworkManager::assignClientID(ENetPeer* peer) {
    std::cout << "CRIANDO ID DO CLIENT" << std::endl;
    sendPacket(peer, MessageType::ASSIGN_ID, &clientIDs[peer], sizeof(int));
    clientIDs[peer] = nextClientID++;
}

void NetworkManager::removeClientID(ENetPeer* peer) {
    std::cout << "DESTRUINDO ID DO CLIENT" << std::endl;
    clientIDs.erase(peer);
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
    address.host = ENET_HOST_ANY;
    address.port = port;

    // Cria um host que pode aceitar até 32 conexões de entrada
    client = enet_host_create(&address, 32, 2, 0, 0);
    if (client == nullptr) {
        std::cerr << "Failed to create an ENet server host.\n";
        return false;
    }

    std::cout << "Hosting a game on port " << port << std::endl;
    isHost = true;
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
            std::cout << "A client connected from " << event.peer->address.host << ":" << event.peer->address.port << ".\n";
            assignClientID(event.peer);
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            if (event.peer != nullptr && event.peer->data != nullptr) {
                std::cout << "A packet of length " << event.packet->dataLength
                    << " containing " << event.packet->data
                    << " was received from " << event.peer->data
                    << " on channel " << event.channelID << ".\n";
                // Clean up the packet now that we're done using it.
                MessageType* messageType = reinterpret_cast<MessageType*>(event.packet->data);
                if (*messageType == MessageType::GAME_READY) {
                    // Definir a flag para iniciar o jogo.
                    readyToStartGame = true;
                }
                enet_packet_destroy(event.packet);
            }
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            if (event.peer != nullptr) {
                std::cout << "Disconnection occurred.\n";
                removeClientID(event.peer);
                event.peer->data = nullptr;
            }
            break;

        default:
            break;
        }
    }
}

void NetworkManager::notifyGameReady() {
    if (isHost) {
        // A mensagem real pode ser apenas o tipo se não precisarmos de dados adicionais.
        MessageType messageType = MessageType::GAME_READY;

        // Notificar todos os clientes conectados.
        for (ENetPeer* peer = client->peers; peer < &client->peers[client->peerCount]; ++peer) {
            // Verifique se o peer está conectado antes de enviar.
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                NetworkManager::sendPacket(peer, messageType, &messageType, sizeof(messageType));
            }
        }
    }
    else {
        // Esta função não seria chamada pelo cliente no uso normal, pois o cliente
        // recebe a notificação do servidor. Você trataria isso no ProcessNetworkEvents.
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

std::map<ENetPeer*, int> NetworkManager::getClientIDs() {
    return clientIDs;
}
