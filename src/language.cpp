#include "language.hpp"

#include <fstream>
#include <iostream>

language::language()
{
    m_context.append({context_item::type::INSTRUCTION, {R"INSTRUCTION(
        You are a helpful assistant to create SQL queries for a PostgreSQL database.
        Output only SQL queries, without markdown.
        Your output will feed a Language Relational Model program.
        )INSTRUCTION"}});
}

language::~language()
{
}

void language::query(const std::string& request)
{
    m_context.append({context_item::type::USER_INPUT, {request}});

    spdlog::info("requesting: {}", request);

    const auto reply = m_model.query(m_context);
    m_context.append({context_item::type::QUERY_INPUT, {reply}});

    spdlog::info("model result:\n{}", reply);

    const auto output = m_database.execute(reply);
    m_context.append({context_item::type::QUERY_RESULT, {output ? output.value() : "Request executed."}});

    spdlog::info("database result:\n{}", output ? output.value() : "Request executed.");
}