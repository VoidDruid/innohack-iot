#include "sensor.h"

using namespace innohack;

constexpr uint8_t lightSensorPin = 15;

Sensor::Sensor()
    : adapters{std::make_tuple<TroykaLight>(TroykaLight(lightSensorPin))} {
    init();
}

void Sensor::init() {
}

Sensor& Sensor::getInstance() {
    static Sensor instance;
    return instance;
}

Sensor::sensor_value_type Sensor::getSensorData(SensorType type) {
    switch(type) {
        case SensorType::humidity: {
            return 40.0;
        }
        case SensorType::light: {
            TroykaLight& lightSensor = std::get<0>(adapters);
            lightSensor.read();
            return lightSensor.getLightLux();
        }
        case SensorType::temperature: {
            return 100.0;
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
