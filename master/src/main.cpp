#include <HTTPClient.h>
#include "metrics.h"
#include "http.h"
#include "bluetooth.h"

static constexpr uint16_t BAUD_RATE = 9600;
static constexpr uint16_t STARTUP_DELAY = 5000;
static constexpr auto ENDPOINT = "http://drop-table.tech/api";

void setup() {
    Serial.begin(BAUD_RATE);
    delay(STARTUP_DELAY);
}
 
void loop() {
    auto metrics = innohack::Metrics::getInstance();
    innohack::SimpleModel metric_model {
        {{"gas", "10"}}
    };
    metrics.reportMetric(metric_model);

    auto& http = innohack::HTTP::getInstance();
    auto result = http.post(ENDPOINT, metric_model);
    if(result.first) {
        Serial.print(result.second);
    } 
    else {
        Serial.print("Error while sending GET");
    }
    metrics.checkQueue();
    auto& bluetooth = innohack::Bluetooth::getInstance();
    auto connected = bluetooth.isConnected();
    if(connected) {
        Serial.print("Connected");
    } 
    else {
        Serial.print("Not connected");
    }
    delay(1000);
}