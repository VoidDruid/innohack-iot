#include "simple_model.h"
#include <sstream>

using namespace innohack;

SimpleModel::SimpleModel(const model_type& model) : m_model{model} {}

String SimpleModel::toJson() {
    std::stringstream json;
    json << "{";
    for(auto elem : m_model) {
        json << "\"" << elem.first.c_str() << "\":" << "\"" << elem.second.c_str() << "\",";
    }
    json << "}";
    return json.str().c_str();
}
SimpleModel::model_type SimpleModel::toMap() {
    return m_model;
}