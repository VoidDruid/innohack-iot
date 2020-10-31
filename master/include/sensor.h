#pragma once

#include <cstdint>
#include <string>
#include <tuple>
#include <TroykaLight.h>

namespace innohack {

class Sensor {
    public:
        static Sensor& getInstance();
        enum class SensorType : uint8_t {
            temperature,
            humidity,
            light
        };
        static std::string to_string(SensorType type);
        using sensor_value_type = float;
        sensor_value_type getSensorData(SensorType type);
    private:
        Sensor();
        void init();
        std::tuple<TroykaLight> adapters;
};
}