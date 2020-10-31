#include "simple_model.h"
#include <sstream>

using namespace innohack;

SimpleModel::SimpleModel(const model_type& model) : m_model{model} {}

std::string SimpleModel::toJson() {
    std::stringstream json;
    json << "{";
    for(auto elem : m_model) {
        json << (elem == *m_model.begin() ? "" : ",") 
        << "\"" << elem.first.c_str() 
        << "\":";
        if(elem.second.find("{") != std::string::npos) {
            json << elem.second.c_str();
        }
        else {
            json << "\"" << elem.second.c_str() << "\"";
        }
    }
    json << "}";
    return json.str().c_str();
}
SimpleModel::model_type SimpleModel::toMap() {
    return m_model;
}