#pragma once

#include <nlohmann/json.hpp>

class responses
{
public:
    responses();
    ~responses();

    void create(const std::string& input);

private:
    nlohmann::json post(const std::string& endpoint, const nlohmann::json& body);

    const std::string m_api_key;
    const std::string m_base_url;
    const std::string m_model;
    const float       m_temperature;
    std::string       m_id;
};