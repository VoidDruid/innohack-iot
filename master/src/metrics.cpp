#include <sstream>

#include "metrics.h"
#include "http.h"

using namespace innohack;

static constexpr auto SERVER_DOMAIN = "http://drop-table.tech";
static constexpr auto METRICS_ENDPOINT = "metrics";
static constexpr uint8_t MAX_RETRIES = 5;

Metrics& Metrics::getInstance() {
    static Metrics metrics;
    return metrics;
}

void Metrics::reportMetric(SimpleModel& metrics) {
    Serial.println("reportMetric()");
    auto& http = HTTP::getInstance();
    std::stringstream endpoint;
    endpoint << SERVER_DOMAIN << "/" << METRICS_ENDPOINT;
    auto result = http.post(String(endpoint.str().c_str()), metrics);
    
    if(!result.first) {
        m_queued_requests.push(metrics);
    }
}

void Metrics::checkQueue() {
    Serial.println("checkQueue()");
    uint8_t current_retry{0};

    while(!m_queued_requests.empty() && current_retry != MAX_RETRIES) {
        auto metric = m_queued_requests.top();
        auto& http = HTTP::getInstance();
        std::stringstream endpoint;
        endpoint << SERVER_DOMAIN << "/" << METRICS_ENDPOINT;
        auto result = http.post(String(endpoint.str().c_str()), metric);
        
        if(result.first) {
            m_queued_requests.pop();
        } 
        else {
            current_retry++;
        }
    }
}