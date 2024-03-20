#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#define NOMINMAX
#include "SFML/Graphics.hpp"
//#include <windows.h>


#include <enet/enet.h>
#include <iostream>
#include <string>
#include <map>
#include <memory>

#include "Context.hpp"

enum class MessageType {
    GAME_READY
};

struct ClientData {
    MessageType start_flag = MessageType::GAME_READY;
};

class NetworkManager : public std::enable_shared_from_this<NetworkManager> {
public:
    NetworkManager(std::shared_ptr <Context>& m_context);
    ~NetworkManager();

    bool StartClient(const std::string& address, uint16_t port);
    void ProcessNetworkEvents();
    void Disconnect();
    bool IsConnected();
    bool isReadyToStartGame();
    bool StartHost(uint16_t port);
    void sendPacket(ENetPeer* peer, MessageType type, const void* data, size_t dataLength);
    void notifyGameReady();

    ENetPeer* getClients() const;
    ENetPeer* peer = nullptr;
    ENetHost* client = nullptr;

private:

    bool isHost = false;
    bool readyToStartGame = false;

    std::shared_ptr <Context>& m_context;
};

#endif // NETWORKMANAGER_H
