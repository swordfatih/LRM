#pragma once

#include "database.hpp"
#include "tool.hpp"

namespace tools
{

class describe : public tool
{
public:
    describe() : tool("describe_table", "Provide detailed metadata about a table in the PostgreSQL database, including columns, keys, and indexes.")
    {
        m_properties.emplace_back("table", "Required. Exact name of the table to describe.");
        m_properties.emplace_back("schema", "Optional. Name of the schema containing the table (default: public).");
    }

    virtual ~describe() = default;

    std::string callback(const std::map<std::string, std::string>& arguments)
    {
        auto table = arguments.at("table");
        auto schema = arguments.contains("schema") ? arguments.at("schema") : "public";

        if(!database::is_valid_identifier(table) || !database::is_valid_identifier(schema))
        {
            return "Invalid schema or table name.";
        }

        auto query = std::format(
            R"QUERY(
                SELECT
                    t.table_name,
                    pg_catalog.obj_description(pgc.oid, 'pg_class') AS table_description,
                    pgc.reltuples::bigint AS approximate_row_count,

                    (
                        SELECT json_agg(json_build_object(
                            'column_name', c.column_name,
                            'data_type', c.data_type,
                            'character_maximum_length', c.character_maximum_length,
                            'column_default', c.column_default,
                            'is_nullable', c.is_nullable,
                            'column_description', col_description(pgc.oid, c.ordinal_position)
                        ) ORDER BY c.ordinal_position)
                        FROM information_schema.columns c
                        WHERE c.table_name = '{}' AND c.table_schema = '{}'
                    ) AS columns,

                    (
                        SELECT array_agg(a.attname ORDER BY a.attnum)
                        FROM pg_index i
                        JOIN pg_attribute a ON a.attrelid = i.indrelid AND a.attnum = ANY(i.indkey)
                        WHERE i.indrelid = pgc.oid AND i.indisprimary
                    ) AS primary_keys,

                    (
                        SELECT json_agg(json_build_object(
                            'column_name', kcu.column_name,
                            'foreign_table_name', ccu.table_name,
                            'foreign_column_name', ccu.column_name
                        ))
                        FROM information_schema.table_constraints tc
                        JOIN information_schema.key_column_usage kcu
                            ON tc.constraint_name = kcu.constraint_name AND tc.table_schema = kcu.table_schema
                        JOIN information_schema.constraint_column_usage ccu
                            ON ccu.constraint_name = tc.constraint_name AND ccu.table_schema = tc.table_schema
                        WHERE tc.constraint_type = 'FOREIGN KEY' AND tc.table_name = '{}' AND tc.table_schema = '{}'
                    ) AS foreign_keys,

                    (
                        SELECT json_agg(json_build_object(
                            'index_name', i.relname,
                            'index_type', am.amname,
                            'columns', array_agg(a.attname ORDER BY a.attnum),
                            'is_unique', ix.indisunique
                        ))
                        FROM pg_class i
                        JOIN pg_index ix ON i.oid = ix.indexrelid
                        JOIN pg_attribute a ON a.attrelid = ix.indrelid AND a.attnum = ANY(ix.indkey)
                        JOIN pg_am am ON i.relam = am.oid
                        WHERE ix.indrelid = pgc.oid
                        GROUP BY i.relname, am.amname, ix.indisunique
                    ) AS indexes

                FROM information_schema.tables t
                JOIN pg_catalog.pg_class pgc ON t.table_name = pgc.relname AND pgc.relkind = 'r'
                WHERE t.table_schema = '{}' AND t.table_type = 'BASE TABLE' AND t.table_name = '{}'
                ORDER BY t.table_name;
            )QUERY",
            table, schema, table, schema, schema, table);

        auto output = m_database.execute(query);
        return output ? output.value() : "Table or schema not found.";
    }

private:
    database m_database;
};

}
