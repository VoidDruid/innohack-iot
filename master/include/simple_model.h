#pragma once

#include <map>
#include "Arduino.h"

namespace innohack {
class SimpleModel {
public:
    using model_type = std::map<std::string, std::string>;
    SimpleModel() = default;
    SimpleModel(const model_type& model);
    std::string toJson();
    model_type toMap();
private:
    model_type m_model;
};
}