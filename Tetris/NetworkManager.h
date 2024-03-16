#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <enet/enet.h>
#include <iostream>
#include <string>

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    bool StartConnection(const std::string& address, uint16_t port);
    void ProcessNetworkEvents();
    void Disconnect();
    bool IsConnected();

private:
    ENetHost* client = nullptr;
    ENetPeer* peer = nullptr;
};

#endif // NETWORKMANAGER_H
