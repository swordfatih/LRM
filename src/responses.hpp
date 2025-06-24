#pragma once

#include "database.hpp"
#include "tool.hpp"
#include <map>
#include <memory>
#include <nlohmann/json.hpp>

class responses
{
public:
    responses();
    ~responses() = default;

    nlohmann::json create(std::vector<nlohmann::json::object_t>& input, std::optional<std::string>& previous_id);

    template <typename tool_t>
    void use()
    {
        auto tool = std::make_unique<tool_t>();
        m_tools.insert({tool->get_name(), std::move(tool)});
    }

private:
    nlohmann::json post(const std::string& endpoint, const nlohmann::json& body);

    database                                     m_database;
    const std::string                            m_api_key;
    const std::string                            m_base_url;
    const std::string                            m_model;
    const float                                  m_temperature;
    std::map<std::string, std::unique_ptr<tool>> m_tools;
};