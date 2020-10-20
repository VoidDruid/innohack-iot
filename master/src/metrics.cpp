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

void Metrics::reportMetric(metric_type& metric) {
    auto& http = HTTP::getInstance();
    String endpoint = SERVER_DOMAIN + String("/") + METRICS_ENDPOINT;
    SimpleModel model {
        { metric.first, metric.second }
    };
    auto result = http.post(endpoint, model);
    
    if(!result.first) {
        m_queued_requests.push(metric);
    }
}

void Metrics::reportMetric(SimpleModel& metrics) {
    auto& http = HTTP::getInstance();
    String endpoint = SERVER_DOMAIN + String("/") + METRICS_ENDPOINT;
    auto result = http.post(endpoint, metrics);
    
    if(!result.first) {
        for(auto metric : metrics.toMap()) {
            m_queued_requests.push(metric);
        }
    }
}

void Metrics::checkQueue() {
    uint8_t current_retry{0};

    while(!m_queued_requests.empty() && current_retry != MAX_RETRIES) {
        auto metric = m_queued_requests.top();
        auto& http = HTTP::getInstance();
        String endpoint = SERVER_DOMAIN + String("/") + METRICS_ENDPOINT;
        SimpleModel model {
            { metric.first, metric.second }
        };
        auto result = http.post(endpoint, model);
        
        if(result.first) {
            m_queued_requests.pop();
        } 
        else {
            current_retry++;
        }
    }
}