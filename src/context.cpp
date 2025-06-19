#include "context.hpp"

context::context()
{
}

context::~context()
{
}

void context::append(const std::string& item)
{
    m_items.push_back(item);
}

void context::append(const std::optional<std::string>& item)
{
    if(item)
    {
        append(item.value());
    }
}

std::string context::format() const
{
    if(m_items.empty())
    {
        return {};
    }

    std::string builder;
    for(auto item: m_items)
    {
        builder += "---BEGIN CONTEXT ITEM---\n" + item + "\n---END CONTEXT ITEM---\n";
    }

    return builder;
}