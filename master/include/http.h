#pragma once

#include <cstring>
#include "simple_model.h"

namespace innohack {
class HTTP {
    public:
        static HTTP& getInstance();
        std::pair<bool, String> post(const String& url, SimpleModel& model);
    private:
        void init();
        HTTP();
        HTTP& operator=(const HTTP&) = delete;
        HTTP(const HTTP&) = delete;
};
}