#include "context.hpp"

context_item::context_item(context_item::type type, const std::vector<std::string>& messages) : m_type(type), m_messages(messages)
{
}

context_item::~context_item()
{
}

nlohmann::json context_item::format() const
{
    std::string merged;

    for(auto message: m_messages)
    {
        merged += message + "\n";
    }

    nlohmann::json object = {
        {"role", role()},
        {"content", merged},
    };

    return object;
}

std::string context_item::role() const
{
    switch(m_type)
    {
    case context_item::type::INSTRUCTION:
        return "developer";
    case context_item::type::QUERY_INPUT:
        return "user";
    case context_item::type::QUERY_RESULT:
        return "assistant";
    case context_item::type::USER_INPUT:
        return "user";
    case context_item::type::USER_RESULT:
        return "assistant";
    default:
        return "tool";
    }
}

context::context()
{
}

context::~context()
{
}

void context::append(const context_item& item)
{
    m_items.push_back(item);
}

nlohmann::json context::format() const
{
    if(m_items.empty())
    {
        return {};
    }

    auto array = nlohmann::json::array();
    for(auto item: m_items)
    {
        array.push_back(item.format());
    }

    return array;
}