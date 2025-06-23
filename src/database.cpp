#include "database.hpp"
#include "environment.hpp"
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

database::~database()
{
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
    }

    return std::nullopt;
}

std::string database::format(const tao::pq::result& result)
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