#pragma once

#include "database.hpp"
#include "tool.hpp"

namespace tools
{

class list : public tool
{
public:
    list() : tool("list_tables", "List user-defined tables in a specified schema of the Postgres database")
    {
        m_properties.emplace_back("schema", "Schema name (default: public)");
    }

    virtual ~list() = default;

    std::string callback(const std::map<std::string, std::string>& arguments)
    {
        auto schema = arguments.contains("schema") ? arguments.at("schema") : "public";

        if(!database::is_valid_identifier(schema))
        {
            return "Invalid schema name.";
        }

        auto query = std::format(
            R"QUERY(
                SELECT 
                    t.table_name,
                    pg_catalog.obj_description(pgc.oid, 'pg_class') AS table_description,
                    STRING_AGG(
                        c.column_name || ' (' || c.data_type || ')',
                        ', ' ORDER BY c.ordinal_position
                    ) AS columns
                FROM information_schema.tables t
                JOIN pg_catalog.pg_class pgc 
                    ON t.table_name = pgc.relname AND pgc.relkind = 'r'
                JOIN information_schema.columns c 
                    ON t.table_name = c.table_name AND t.table_schema = c.table_schema
                WHERE t.table_type = 'BASE TABLE' AND t.table_schema = '{}'
                GROUP BY t.table_name, pgc.oid
                ORDER BY t.table_name;
            )QUERY",
            schema);

        auto output = m_database.execute(query);
        return output ? output.value() : std::format("No tables found in schema '{}'.", schema);
    }

private:
    database m_database;
};

}
