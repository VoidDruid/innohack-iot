#pragma once

#include <HTTPClient.h>
#include <cstring>
#include "simple_model.h"

namespace innohack {
class HTTP {
    public:
        static HTTP& getInstance();

        template<typename Response>
        std::pair<bool, Response> get(const String& url) {
            if ((WiFi.status() == WL_CONNECTED)) {
                HTTPClient http;
                http.begin(url.c_str());
                int httpCode = http.GET();
                if (httpCode > 0) {
                    String payload = http.getString();
                    return std::make_pair<bool, Response>(true, Response(payload));
                }
                http.end();
            }
            return std::make_pair<bool, Response>(false, Response());
        }
        std::pair<bool, String> post(const String& url, SimpleModel& model);
    private:
        void init();
        HTTP() = default;
};
}