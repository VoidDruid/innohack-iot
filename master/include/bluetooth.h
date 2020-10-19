#pragma once

#include <BLEDevice.h>

namespace innohack {
class Bluetooth : public BLEServerCallbacks { 
    public:
        static Bluetooth& getInstance();
        void onConnect(BLEServer* pServer);
        void onDisconnect(BLEServer* pServer);
        bool isConnected() { return m_isConnected; }
    private:
        void init();
        Bluetooth();
        Bluetooth& operator=(const Bluetooth&) = delete;
        Bluetooth(const Bluetooth&) = delete;

        BLEServer *m_server{nullptr};
        BLEService *m_service{nullptr};
        BLECharacteristic *m_characteristic{nullptr};
        bool m_isConnected{false};
};
}