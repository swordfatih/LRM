#include "language.hpp"

#include <cpr/cpr.h>
#include <fstream>
#include <iostream>

language::language() : m_base_url("https://api.openai.com/v1/")
{
    auto vars = retrieve_env();
    m_api_key = vars["api_key"];

    m_connection = tao::pq::connection::create("dbname=lrm host=45.140.164.225 user=example password=example");
}

language::~language()
{
}

std::map<std::string, std::string> language::retrieve_env(const std::string& path)
{
    std::ifstream file(path);

    if(!file)
    {
        throw std::runtime_error("Error: Could not open .env file at " + path);
    }

    std::map<std::string, std::string> vars;
    std::string                        line;
    while(std::getline(file, line))
    {
        if(line.find("api_key=") == 0)
        {
            auto key = line.substr(0, line.find('='));
            auto value = line.substr(line.find('=') + 1);
            vars[key] = value;
        }
    }

    return vars;
}

std::string language::chat(const nlohmann::json& messages)
{
    nlohmann::json request_body = {
        {"model", "gpt-3.5-turbo"},
        {"messages", messages},
        {"max_tokens", 500},
        {"temperature", 0.7}};

    cpr::Response r = cpr::Post(cpr::Url{m_base_url + "chat/completions"},
                                cpr::Header{{"Authorization", "Bearer " + m_api_key},
                                            {"Content-Type", "application/json"}},
                                cpr::Body{request_body.dump()},
                                cpr::Timeout{10000});

    nlohmann::json response_json = nlohmann::json::parse(r.text);
    std::string    reply = response_json["choices"][0]["message"]["content"];

    spdlog::debug("chat reply:\n{}", reply);

    return reply;
}

std::string language::ai_query(const std::string& request)
{
    // clang-format off
    nlohmann::json messages = {
        {
            {"role", "system"},
            {"content", 
                R"system(
                    You are a helpful assistant to create SQL queries for a PostgreSQL database.
                    Output only SQL queries, without markdown.
                    Your output will feed a Language Relational Model program.
                )system"
            }
        },
        {
            {"role", "user"},
            {"content", 
                m_context.format() 
                + "\n---BEGIN REQUEST---\n" 
                + request 
                + "\n---END REQUEST---\n"
            }
        }
    };
    // clang-format on

    spdlog::debug("chat messages:\n{}", messages.dump());

    return chat(messages);
}

std::optional<std::string> language::db_query(const std::string& request)
{
    try
    {
        auto reply = ai_query(request);
        const auto result = m_connection->execute(reply);

        if(result.has_rows_affected())
        {
            spdlog::info("rows affected: {}", result.rows_affected());
        }

        std::optional<std::string> output;
        if(result.columns() > 0)
        {
            auto formatted = format_result(result);
            output = std::make_optional<nlohmann::json>(formatted);

            spdlog::debug("db query result:\n{}", formatted);
        }

        m_context.append(output);

        return output;
    }
    catch(tao::pq::sql_error e)
    {
        spdlog::error("sql error:\n{}", e.what());
    }

    return std::nullopt;
}

std::string language::format_result(const tao::pq::result& result)
{
    std::string formatted;
    for(const auto& row: result)
    {
        for(const auto& field: row)
        {
            formatted += field.name() + ": " + field.as<std::string>() + "; ";
        }

        formatted += "\n";
    }

    return formatted;
}