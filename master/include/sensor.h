#pragma once

#include <cstdint>
#include <string>

namespace innohack {

class Sensor {
    public:
    enum class SensorType : uint8_t {
        temperature,
        humidity,
        light
    };
    static std::string to_string(SensorType type);
    using sensor_value_type = uint32_t;
    static sensor_value_type getSensorData(SensorType type);
};
}