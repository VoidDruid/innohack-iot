#pragma once

#include <cstdint>
#include <string>
#include <tuple>
#include <TroykaLight.h>
#include <TroykaDHT.h>

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
        std::tuple<TroykaLight, DHT> adapters;
        std::pair<float, float> getDHTData();
        DHT& getDHT();
        TroykaLight& getLight();
};
}