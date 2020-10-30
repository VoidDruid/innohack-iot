#include <Arduino.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLE2902.h>

#include "bluetooth.h"

using namespace innohack;

constexpr const char* BLE_SERVER_NAME = "INNOHACK";
constexpr const char* SERVICE_UUID = "91bad492-b950-4226-aa2b-4ede9fa42f59";
constexpr const char* CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
constexpr uint8_t ACCESS = BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE;

struct Property {
    const char * name;
    BLECharacteristic * characteristic;
    const char * UUID;    
    BLEDescriptor descriptor;
};
std::array<Property, 3> properties = {
    Property{ "temperature", nullptr, "beb5483e-36e1-4688-b7f5-ea07361b26a8", BLEUUID((uint16_t)0x2903)},
    Property{ "humidity", nullptr, "beb5483e-36e1-4688-b7f5-ea07361b26a7", BLEUUID((uint16_t)0x2902) },
    Property{ "light", nullptr, "beb5483e-36e1-4688-b7f5-ea07361b26a6", BLEUUID((uint16_t)0x2901) }
};

void BluetoothServer::init() {
    BLEDevice::init(BLE_SERVER_NAME);

    m_server = BLEDevice::createServer();
    m_server->setCallbacks(this);
    
    m_service = m_server->createService(SERVICE_UUID);
    
    for(auto& property : properties) {
        property.characteristic = m_service->createCharacteristic(property.UUID, ACCESS);
        property.descriptor.setValue(property.name);
        property.characteristic->addDescriptor(&property.descriptor);
    }

    m_service->start();
    m_server->getAdvertising()->start();
}
bool BluetoothServer::isConnected() {
    return m_isConnected;
}
void BluetoothServer::run() {
    if (m_isConnected) {
        for(auto& property : properties) {
            

            Serial.printf("*** NOTIFY: %d ***\n", value);
            property.characteristic->setValue(&value, 1);
            property.characteristic->notify();
            //pCharacteristic->indicate();
            value++;
        }
        
    }
}

void BluetoothServer::onConnect(BLEServer* pServer) {
    m_isConnected = true;
}

void BluetoothServer::onDisconnect(BLEServer* pServer) {
    m_isConnected = false;
}

BluetoothServer::BluetoothServer() {
    init();
}

BluetoothServer& BluetoothServer::getInstance() {
    static BluetoothServer instance;
    return instance;
}
