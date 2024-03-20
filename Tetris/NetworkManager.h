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
    MSG_ATTACK
};

struct ClientData {
    MessageType flag;
    int blocksToSend;
};

class NetworkManager : public std::enable_shared_from_this<NetworkManager> {
public:
    NetworkManager(std::shared_ptr <Context>& m_context);
    ~NetworkManager();

    bool StartClient(const std::string& address, uint16_t port);
    void ProcessNetworkEvents();
    void Disconnect();
    bool IsConnected();
    bool StartHost(uint16_t port);
    void sendPacket(ENetPeer* peer, const ClientData& messageData);

    void notifyGameReady();
    void notifyAttack(int blocksCount);

    bool isReadyToStartGame();
    bool getAttackStatus();
    int getLinesAdded();

    ENetPeer* getClients() const;
    ENetPeer* peer = nullptr;
    ENetHost* client = nullptr;

private:

    bool isHost = false;
    bool readyToStartGame = false;
    bool attackStatus = false;

    int linesAdded = 0;

    std::shared_ptr <Context>& m_context;
};

#endif // NETWORKMANAGER_H
