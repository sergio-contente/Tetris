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
    GAME_READY,
    ASSIGN_ID
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
    void assignClientID(ENetPeer* peer);
    void removeClientID(ENetPeer* peer);
    void sendPacket(ENetPeer* peer, MessageType type, const void* data, size_t dataLength);
    void notifyGameReady();

    std::map<ENetPeer*, int> getClientIDs();

private:
    ENetHost* client = nullptr;
    ENetPeer* peer = nullptr;

    std::map<ENetPeer*, int> clientIDs;
    int nextClientID = 1;  // Starting from 1 for client IDs
    bool readyToStartGame = false;
    bool isHost = false;

    std::shared_ptr <Context>& m_context;
};

#endif // NETWORKMANAGER_H
