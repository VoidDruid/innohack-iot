#include <Arduino.h>
#include <BLEUtils.h>
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
constexpr const uint8_t notificationOn[] = {0x1, 0x0};
constexpr const uint8_t notificationOff[] = {0x0, 0x0};

struct Property {
    const char * name;
    BLECharacteristic *characteristic;
    const char * UUID;    
    BLEDescriptor descriptor;
    BLERemoteCharacteristic *remoteCharacteristic;
    void (*callback)(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                        uint8_t* pData, size_t length, bool isNotify);
};
std::array<Property, 3> properties = {
    Property{ "temperature", nullptr, "beb5483e-36e1-4688-b7f5-ea07361b26a8", BLEUUID((uint16_t)0x2903), nullptr, BluetoothClient::temperatureNotifyCallback },
    Property{ "humidity", nullptr, "beb5483e-36e1-4688-b7f5-ea07361b26a7", BLEUUID((uint16_t)0x2902), nullptr, BluetoothClient::temperatureNotifyCallback },
    Property{ "light", nullptr, "beb5483e-36e1-4688-b7f5-ea07361b26a6", BLEUUID((uint16_t)0x2901), nullptr, BluetoothClient::temperatureNotifyCallback }
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

BluetoothClient& BluetoothClient::getInstance() {
    static BluetoothClient instance;
    return instance;
}

BluetoothClient::BluetoothClient() {
    init();
}

void BluetoothClient::init() {
    BLEDevice::init("");

    m_BLEScan = BLEDevice::getScan();
    m_BLEScan->setAdvertisedDeviceCallbacks(this);
    m_BLEScan->setActiveScan(true);
    m_BLEScan->start(30);

    for(auto& property : properties) {
        property.callback = temperatureNotifyCallback;
    }
}

void BluetoothClient::onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == BLE_SERVER_NAME) {
      advertisedDevice.getScan()->stop(); 
      m_serverAddress = new BLEAddress(advertisedDevice.getAddress()); 
      m_doConnect = true;
      Serial.println("Device found. Connecting!");
    }
}
void cc(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                        uint8_t* pData, size_t length, bool isNotify) {
    Serial.print("Temperature: ");
    Serial.printf("here %d", *pData);
}

bool BluetoothClient::connectToServer() {
    BLEClient* pClient = BLEDevice::createClient();
    Serial.println("here");
    pClient->connect(*m_serverAddress);
    
    Serial.println(" - Connected to server");
    BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
    if (pRemoteService == nullptr) {
        Serial.print("Failed to find our service UUID: ");
        Serial.println(SERVICE_UUID);
        return (false);
    }
    
    for(auto& property : properties) {
        Serial.printf("here2 %s", property.UUID);
        auto* chara = pRemoteService->getCharacteristic(property.UUID);
        if(!chara) {
            Serial.println("Failed to find our characteristic UUID");
            return false;
        }
        Serial.println("here callback");
        Serial.printf("%d", chara);
        chara->registerForNotify(cc);
        Serial.println("here callback end");
    }
    Serial.println("here3");
    Serial.println(" - Found our characteristics");
    return true;
}

void BluetoothClient::temperatureNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                        uint8_t* pData, size_t length, bool isNotify) {
  Serial.print("Temperature: ");
  Serial.printf("here %d", *pData);
}
void BluetoothClient::run() {
    if (m_doConnect == true) {
        if (connectToServer()) {
            Serial.println("We are now connected to the BLE Server.");
            for(auto& property : properties) {
                //property.remoteCharacteristic->getDescriptor(property.descriptor.getUUID())->writeValue((uint8_t*)notificationOn, 2, true);
            }
            m_connected = true;
        } 
        else {
            Serial.println("We have failed to connect to the server; Restart your device to scan for nearby BLE server again.");
        }
        m_doConnect = false;
  }
}