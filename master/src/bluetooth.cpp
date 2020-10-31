#include <Arduino.h>
#include <BLEUtils.h>
#include <BLEAdvertisedDevice.h>
#include <BLE2902.h>
#include <cstdint>
#include <sstream>

#include "bluetooth.h"
#include "sensor.h"
#include "simple_model.h"
#include "metrics.h"

using namespace innohack;

constexpr const char* BLE_SERVER_NAME = "vLLjC7SXRuKo6SeqrlTZtA";
constexpr const char* SERVICE_UUID = "91bad492-b950-4226-aa2b-4ede9fa42f59";
constexpr const char* CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
constexpr uint8_t ACCESS = BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE;
constexpr const uint8_t notificationOn[] = {0x1, 0x0};
constexpr const uint8_t notificationOff[] = {0x0, 0x0};

struct Property {
    Sensor::SensorType type;
    BLECharacteristic *characteristic;
    const char * UUID;    
    BLEDescriptor descriptor;
    BLERemoteCharacteristic *remoteCharacteristic;
    void (*callback)(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                        uint8_t* pData, size_t length, bool isNotify);
    Sensor::sensor_value_type value;
};

std::array<Property, 3> properties = {
    Property{ Sensor::SensorType::temperature, nullptr, "beb5483e-36e1-4688-b7f5-ea07361b26a8", BLEUUID((uint16_t)0x2903), nullptr, BluetoothClient::temperatureNotifyCallback, 0 },
    Property{ Sensor::SensorType::humidity, nullptr, "beb5483e-36e1-4688-b7f5-ea07361b26a7", BLEUUID((uint16_t)0x2902), nullptr, BluetoothClient::humidityNotifyCallback, 0 },
    Property{ Sensor::SensorType::light, nullptr, "beb5483e-36e1-4688-b7f5-ea07361b26a6", BLEUUID((uint16_t)0x2901), nullptr, BluetoothClient::ligthNotifyCallback, 0 }
};

SimpleModel::model_type metric{};

void BluetoothServer::init() {
    BLEDevice::init(BLE_SERVER_NAME);

    m_server = BLEDevice::createServer();
    m_server->setCallbacks(this);
    
    m_service = m_server->createService(SERVICE_UUID);
    
    for(auto& property : properties) {
        property.characteristic = m_service->createCharacteristic(property.UUID, ACCESS);
        property.characteristic->addDescriptor(&property.descriptor);
    }

    m_service->start();
    m_server->getAdvertising()->start();
}

bool BluetoothServer::isConnected() {
    return m_isConnected;
}

void fillPropertiesWithData() {

}

void BluetoothServer::run() {
    if (m_isConnected) {
        for(auto& property : properties) {
            property.value = Sensor::getSensorData(property.type);
            Serial.printf("%s: %d\n", Sensor::to_string(property.type).c_str(), property.value);
            property.characteristic->setValue(property.value);
            property.characteristic->notify();
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
}

void BluetoothClient::onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == BLE_SERVER_NAME) {
      advertisedDevice.getScan()->stop(); 
      m_serverAddress = new BLEAddress(advertisedDevice.getAddress()); 
      m_doConnect = true;
      Serial.println("Device found. Connecting!");
    }
}

bool BluetoothClient::connectToServer() {
    BLEClient* pClient = BLEDevice::createClient();
    pClient->connect(*m_serverAddress);
    
    Serial.println(" - Connected to server");
    BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
    if (pRemoteService == nullptr) {
        Serial.print("Failed to find our service UUID: ");
        Serial.println(SERVICE_UUID);
        return (false);
    }
    
    for(auto& property : properties) {
        property.remoteCharacteristic = pRemoteService->getCharacteristic(property.UUID);
        if(!property.remoteCharacteristic) {
            Serial.println("Failed to find our characteristic UUID");
            return false;
        }
        property.remoteCharacteristic->registerForNotify(property.callback);
    }
    Serial.println(" - Found our characteristics");
    return true;
}

void BluetoothClient::temperatureNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                        uint8_t* pData, size_t length, bool isNotify) {
    uint32_t data = *((uint32_t*)pData);

    Serial.print("temperature: ");
    Serial.printf("%d\n", data);

    std::stringstream ss;    
    ss << data;  
    std::string str = ss.str();
    auto pair = std::make_pair<std::string, std::string>(
        "temperature", ss.str()
    );
    metric.emplace(pair);
}

void BluetoothClient::ligthNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                        uint8_t* pData, size_t length, bool isNotify) {
    uint32_t data = *((uint32_t*)pData);
    Serial.print("light: ");
    Serial.printf("%d\n", data);

    std::stringstream ss;    
    ss << data;  
    std::string str = ss.str();
    auto pair = std::make_pair<std::string, std::string>(
        "light", ss.str()
    );
    metric.emplace(pair);
}

void BluetoothClient::humidityNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                        uint8_t* pData, size_t length, bool isNotify) {
    uint32_t data = *((uint32_t*)pData);
    Serial.print("humidity: ");
    Serial.printf("%d\n", data);
    
    std::stringstream ss;    
    ss << data;  
    std::string str = ss.str();
    auto pair = std::make_pair<std::string, std::string>(
        "humidity", ss.str()
    );
    metric.emplace(pair);
}

void BluetoothClient::run() {
    constexpr uint8_t SEND_METRIC_PERIOD = 5;
    static uint8_t timer = 0;

    if (m_doConnect == true) {
        if (connectToServer()) {
            Serial.println("We are now connected to the BLE Server.");
            for(auto& property : properties) {
                property.remoteCharacteristic->getDescriptor(property.descriptor.getUUID())->writeValue((uint8_t*)notificationOn, 2, true);
            }
            m_connected = true;
        } 
        else {
            Serial.println("We have failed to connect to the server; Restart your device to scan for nearby BLE server again.");
        }
        m_doConnect = false;
    }

    if(timer == SEND_METRIC_PERIOD && !metric.empty()) {
        metric.emplace(std::make_pair<std::string, std::string>(
            "uuid", BLE_SERVER_NAME
        ));
        SimpleModel model{metric};
        Metrics::getInstance().reportMetric(model);
        timer = 0;
    }
    timer++;
}