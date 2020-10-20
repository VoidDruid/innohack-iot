#include "http.h"

using namespace innohack;
static constexpr auto ssid = "Spaceship";
static constexpr auto password = "private147852";
static constexpr uint16_t DELAY = 1000;

void HTTP::init() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(DELAY);
        Serial.println("Connecting to Wi-Fi..");
    }
    Serial.println("Connected to the Wi-Fi network");
}
HTTP::HTTP() {
    init();
}

HTTP& HTTP::getInstance() {
    static HTTP http;
    return http;
}

std::pair<bool, String> HTTP::post(const String& url, SimpleModel& model) {
    if ((WiFi.status() == WL_CONNECTED)) {
        HTTPClient http;
        http.begin(url.c_str());

        auto payload = model.toJson().c_str();
        auto payloadSize = model.toJson().length();
        int httpCode = http.POST((uint8_t*)payload, payloadSize);
        if (httpCode > 0) {
            String payload = http.getString();
            return std::make_pair<bool, String>(true, String(payload));
        }
        http.end();
    }
    return std::make_pair<bool, String>(false, String());
}
