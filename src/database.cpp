#include "database.hpp"
#include "environment.hpp"
#include <algorithm>
#include <cctype>
#include <format>
#include <spdlog/spdlog.h>

database::database()
{
    m_connection = tao::pq::connection::create(
        std::format("dbname={} host={} user={} password={}",
                    environment::get().at("postgres_db"),
                    environment::get().at("postgres_host"),
                    environment::get().at("postgres_user"),
                    environment::get().at("postgres_password")));
}

std::optional<std::string> database::execute(const std::string& query)
{
    try
    {
        auto result = m_connection->execute(query);

        if(result.has_rows_affected())
        {
            spdlog::debug("(database.cpp) rows affected: {}", result.rows_affected());
        }

        std::optional<std::string> output;
        if(result.columns() > 0)
        {
            auto formatted = format(result);
            output = std::make_optional<nlohmann::json>(formatted);
        }

        return output;
    }
    catch(tao::pq::sql_error e)
    {
        spdlog::error("(database.cpp) sql error:\n{}", e.what());
        return std::format("error with the query: {}", e.what());
    }
}

std::string database::format(const tao::pq::result& result)
{
    nlohmann::json output = nlohmann::json::array();

    for(const auto& row: result)
    {
        nlohmann::json json_row;
        for(const auto& field: row)
        {
            if(field.is_null())
                json_row[field.name()] = nullptr;
            else
                json_row[field.name()] = field.as<std::string>();
        }
        output.push_back(json_row);
    }

    return output.dump(2);
}

bool database::is_valid_identifier(const std::string& ident)
{
    if(ident.empty() || std::isdigit(ident[0]))
        return false;

    return std::all_of(ident.begin(), ident.end(), [](char c)
    {
        return std::isalnum(c) || c == '_';
    });
}