#include <HTTPClient.h>
#include "metrics.h"
#include "http.h"

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

    innohack::Metrics::metric_type metric {"gas", "10"};
    metrics.reportMetric(metric);

    auto http = innohack::HTTP::getInstance();
    auto result = http.get<String>(ENDPOINT);
    if(result.first) {
        Serial1.print(result.second);
    } 
    else {
        Serial1.print("Error while sending GET");
    }
}