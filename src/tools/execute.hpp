#pragma once

#include "database.hpp"
#include "tool.hpp"
#include <spdlog/spdlog.h>

namespace tools
{

class execute : public tool
{
public:
    execute() : tool("execute_sql", "Execute a raw SQL statement (DDL, DML, DCL, TCL) against the Postgres database")
    {
        m_properties.emplace_back("sql", "SQL statement to execute (e.g., SELECT, INSERT, CREATE, GRANT, COMMIT)");
    }

    virtual ~execute() = default;

    std::string callback(const std::map<std::string, std::string>& arguments)
    {
        if(!arguments.contains("sql"))
        {
            return "Missing required argument: 'sql'";
        }

        const std::string& sql = arguments.at("sql");

        if(sql.empty())
        {
            return "Provided SQL query is empty.";
        }

        spdlog::debug("(execute.cpp) Executing SQL: {}", sql);

        auto output = m_database.execute(sql);
        return output.has_value() ? output.value() : "Query executed, no output returned.";
    }

private:
    database m_database;
};

}
