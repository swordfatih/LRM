#pragma once

#include "context.hpp"
#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <tao/pq.hpp>
#include <spdlog/spdlog.h>

class language
{
public:
    language();
    ~language();

    std::string                ai_query(const std::string& request);
    std::optional<std::string> db_query(const std::string& request);

private:
    std::string                        chat(const nlohmann::json& messages);
    std::map<std::string, std::string> retrieve_env(const std::string& path = ".env");
    std::string                        format_result(const tao::pq::result& result);

    context                              m_context;
    std::shared_ptr<tao::pq::connection> m_connection;
    std::string                          m_base_url;
    std::string                          m_api_key;
};