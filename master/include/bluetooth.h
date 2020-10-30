#pragma once

#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

namespace innohack {
class BluetoothServer : public BLEServerCallbacks { 
    public:
        static BluetoothServer& getInstance();
        bool isConnected();
        void run();
    private:
        void init();
        BluetoothServer();
        BluetoothServer& operator=(const BluetoothServer&) = delete;
        BluetoothServer(const BluetoothServer&) = delete;
        void onConnect(BLEServer* pServer);
        void onDisconnect(BLEServer* pServer);

        BLEServer *m_server{nullptr};
        BLEService *m_service{nullptr};
        bool m_isConnected{false};
        uint8_t value{};
};

class BluetoothClient : public BLEAdvertisedDeviceCallbacks {
    public:
        static BluetoothClient& getInstance();
        void run();
        static void temperatureNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                        uint8_t* pData, size_t length, bool isNotify);
    private:
        void init();
        BluetoothClient();
        BluetoothClient& operator=(const BluetoothClient&) = delete;
        BluetoothClient(const BluetoothClient&) = delete;

        void onResult(BLEAdvertisedDevice advertisedDevice);
        bool connectToServer();

        BLEScan *m_BLEScan{nullptr};
        BLEAddress *m_serverAddress{nullptr};
        bool m_doConnect{false};
        bool m_connected{false};
};
}