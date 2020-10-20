#pragma once

#include <utility>
#include "Arduino.h"
#include <stack>

#include "simple_model.h"

namespace innohack {
class Metrics {
    public:
        using metric_type = std::pair<String, String>;
        static Metrics& getInstance();
        void reportMetric(metric_type& metric);
        void reportMetric(SimpleModel& metric);
        void checkQueue();
    private:
        Metrics() = default;
        std::stack<metric_type> m_queued_requests;
};
}