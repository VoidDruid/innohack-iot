#include <Arduino.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLE2902.h>

#include "bluetooth.h"

using namespace innohack;

constexpr const char* BLE_SERVER_NAME = "MyESP32";
constexpr const char* SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
constexpr const char* CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";

void Bluetooth::init() {
    //Serial.print("1");
    BLEDevice::init(BLE_SERVER_NAME);

    m_server = BLEDevice::createServer();
    m_server->setCallbacks(this);
    m_service = m_server->createService(SERVICE_UUID);
    m_characteristic = m_service->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );
    m_characteristic->addDescriptor(new BLE2902());

    m_service->start();
    m_server->getAdvertising()->start();
}

void Bluetooth::onConnect(BLEServer* pServer) {
    m_isConnected = true;
}

void Bluetooth::onDisconnect(BLEServer* pServer) {
    m_isConnected = false;
}

Bluetooth::Bluetooth() {
    init();
}

Bluetooth& Bluetooth::getInstance() {
    static Bluetooth instance;
    return instance;
}
