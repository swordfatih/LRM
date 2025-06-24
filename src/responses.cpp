#include "responses.hpp"
#include "environment.hpp"
#include <cpr/cpr.h>
#include <format>
#include <spdlog/spdlog.h>

responses::responses() : m_api_key(environment::get().at("api_key")),
                         m_base_url("https://api.openai.com/v1/"),
                         m_model("gpt-4.1-nano"),
                         m_temperature(1.f)
{
}

nlohmann::json responses::post(const std::string& endpoint, const nlohmann::json& body)
{
    cpr::Response r = cpr::Post(cpr::Url{m_base_url + endpoint},
                                cpr::Header{{"Authorization", std::format("Bearer {}", m_api_key)},
                                            {"Content-Type", "application/json"}},
                                cpr::Body{body.dump()});

    auto response = nlohmann::json::parse(r.text);

    return response;
}

nlohmann::json responses::create(std::vector<nlohmann::json::object_t>& input, std::optional<std::string>& previous_id)
{
    nlohmann::json body = {
        {"model", m_model},
        {"input", input},
        {"previous_response_id", previous_id},
        {"instructions", R"INSTRUCTIONS(
            You are an interface between the user and a PostgreSQL database.
            Your job is to convert natural language requests into precise tool calls and automatically complete all data fields using your knowledge or best guess.

            Rules:
            Execute all user commands immediately without asking for confirmation or clarification.
            Use only actual database schema elements. Never fabricate tables, columns, or values.
            Always verify existence of tables and rows before assuming anything.
            Always re-check the actual database state before every response. Never rely on memory or cached results.
            Fill in all missing field values automatically using logic, defaults, or invented data. No nulls.
            Always output exactly in the format the user requests. If JSON is requested, return valid and complete JSON.
            Never summarize, never explain unless explicitly asked. Only perform and respond with results.
            Always assume responsibility for ensuring database consistency, completeness, and realism.
            Do not act like a chatbot. You are a deterministic language-to-database interface.     
        )INSTRUCTIONS"}};

    for(auto& [name, tool]: m_tools)
    {
        body["tools"].push_back(tool->format());
    }

    spdlog::debug("(responses.cpp:create) body:\n{}", body.dump());

    auto response = post("responses", body);

    if(response.contains("error") && response.at("error") != nullptr)
    {
        spdlog::error("(responses.cpp:create) api:\n{}", response.at("error").dump());
        return nullptr;
    }

    previous_id = response.at("id");
    auto outputs = response.at("output");

    spdlog::debug("(responses.cpp:create) output:\n{}", outputs.dump());

    bool answer = false;
    for(auto& output: outputs)
    {
        auto type = output.at("type").get<std::string>();
        if(type == "function_call")
        {
            auto name = output.at("name").get<std::string>();
            auto call_id = output.at("call_id").get<std::string>();
            auto arguments_raw = output.at("arguments").get<std::string>();
            auto arguments_parsed = nlohmann::json::parse(arguments_raw).get<std::map<std::string, std::string>>();

            auto result = m_tools.at(name)->callback(arguments_parsed);

            input.push_back({{"type", "function_call_output"},
                             {"call_id", call_id},
                             {"output", result}});

            answer = true;
        }
        else if(type == "message")
        {
            auto content = output.at("content");
            return content.front().at("text");
        }
    }

    return answer ? create(input, previous_id) : nullptr;
}