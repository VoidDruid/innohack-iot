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