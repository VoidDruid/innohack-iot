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
 #define CLIENT
void loop() {
    /*auto metrics = innohack::Metrics::getInstance();
    innohack::SimpleModel metric_model {
        {{"gas", "10"}}
    };
    metrics.reportMetric(metric_model);

    auto& http = innohack::HTTP::getInstance();
    auto result = http.post(ENDPOINT, metric_model);
    if(result.first) {
        Serial.println(result.second);
    } 
    else {
        Serial.println("Error while sending GET");
    }
    metrics.checkQueue();*/
    #ifdef SERVER 
    auto& bluetooth = innohack::BluetoothServer::getInstance();
    auto connected = bluetooth.isConnected();
    if(connected) {
        Serial.println("Connected");
    } 
    else {
        Serial.println("Not connected");
    }
    bluetooth.run();
    #endif
    #ifdef CLIENT
    auto& bluetooth = innohack::BluetoothClient::getInstance();
    bluetooth.run();
    #endif
    delay(1000);
}