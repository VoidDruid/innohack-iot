#include "sensor.h"

using namespace innohack;

Sensor::sensor_value_type Sensor::getSensorData(SensorType type) {
    static int humidity = 0;
    static int light = 0;
    static int temperature = 0;

    switch(type) {
        case SensorType::humidity: {
            return humidity+=10;
        }
        case SensorType::light: {
            return light+=100;
        }
        case SensorType::temperature: {
            return temperature+=1000;
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