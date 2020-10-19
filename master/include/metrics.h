#pragma once

#include <utility>
#include "Arduino.h"
#include <stack>

#include "simple_model.h"

namespace innohack {
class Metrics {
    public:
        static Metrics& getInstance();
        void reportMetric(SimpleModel& metric);
        void checkQueue();
    private:
        Metrics() = default;
        std::stack<SimpleModel> m_queued_requests;
};
}