#pragma once

#include "context.hpp"
#include "environment.hpp"
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <string>

class model
{
public:
    model();
    ~model();
    std::string query(const context& context);

private:
    std::string m_base_url;
    std::string m_api_key;
    std::string m_model;
    float       m_temperature;
    int         m_max_tokens;
};