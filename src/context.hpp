#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

class context_item
{
public:
    enum class type
    {
        INSTRUCTION,
        QUERY_INPUT,
        QUERY_RESULT,
        USER_INPUT,
        USER_RESULT
    };

    context_item(type type, const std::vector<std::string>& messages);
    ~context_item();

    nlohmann::json format() const;

private:
    std::string role() const;

    type             m_type;
    std::vector<std::string> m_messages;
};

class context
{
public:
    context();
    ~context();

    void append(const context_item& item);

    nlohmann::json format() const;

private:
    std::vector<context_item> m_items;
};