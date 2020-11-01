#include "sensor.h"

using namespace innohack;

constexpr uint8_t lightSensorAnalogPin = 34;
constexpr uint8_t dhtSensorDigitalPin = 15;

Sensor::Sensor()
    : adapters{std::make_tuple<TroykaLight, DHT>(TroykaLight(lightSensorAnalogPin), DHT(dhtSensorDigitalPin, DHT11))} {
    init();
}

void Sensor::init() {
    getDHT().begin();
}

std::pair<float, float> Sensor::getDHTData() {
    auto& dht = getDHT();
    dht.read();
    switch(dht.getState()) {
        case DHT_OK: {
            return std::make_pair<float, float>(dht.getTemperatureC(), dht.getHumidity());
        }
        case DHT_ERROR_CHECKSUM: {
            Serial.println("DHT: Checksum error");
            break;
        }
        case DHT_ERROR_TIMEOUT: {
            Serial.println("DHT: Time out error");
            break;
        }
        case DHT_ERROR_NO_REPLY: {
            Serial.println("DHT: Sensor not connected");
            break;
        }
    return std::make_pair<float, float>(NAN, NAN);
    }
}

DHT& Sensor::getDHT() {
    return std::get<1>(adapters);
}

TroykaLight& Sensor::getLight() {
    return std::get<0>(adapters);
}

Sensor& Sensor::getInstance() {
    static Sensor instance;
    return instance;
}

Sensor::sensor_value_type Sensor::getSensorData(SensorType type) {
    switch(type) {
        case SensorType::humidity: {
            return getDHTData().second;
        }
        case SensorType::light: {
            TroykaLight& lightSensor = getLight();
            lightSensor.read();
            return lightSensor.getLightLux();
        }
        case SensorType::temperature: {
            return getDHTData().first;
        }
    }
}

std::string Sensor::to_string(Sensor::SensorType type) {
    switch(type) {
        case SensorType::humidity: {
            return "humidity";
        }
        case SensorType::light: {
            return "light";
        }
        case SensorType::temperature: {
            return "temperature";
        }
    }
}
