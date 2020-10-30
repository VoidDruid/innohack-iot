#pragma once

#include <BLEDevice.h>

namespace innohack {
class BluetoothServer : public BLEServerCallbacks { 
    public:
        static BluetoothServer& getInstance();
        void onConnect(BLEServer* pServer);
        void onDisconnect(BLEServer* pServer);
        bool isConnected();
        void run();
    private:
        void init();
        BluetoothServer();
        BluetoothServer& operator=(const BluetoothServer&) = delete;
        BluetoothServer(const BluetoothServer&) = delete;

        BLEServer *m_server{nullptr};
        BLEService *m_service{nullptr};
        bool m_isConnected{false};
        uint8_t value{};
};
}