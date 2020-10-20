#pragma once

#include <map>
#include "Arduino.h"

namespace innohack {
class SimpleModel {
using model_type = std::map<String, String>;
public:
    SimpleModel() = default;
    SimpleModel(const model_type& model);
    String toJson();
    model_type toMap();
private:
    model_type m_model;
};
}