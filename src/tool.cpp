#include "tool.hpp"

void to_json(nlohmann::json& object, const property& property)
{
    object = nlohmann::json{
        {"type", property.type},
        {"description", property.description},
    };

    for(auto item: property.enumeration)
    {
        object.front()["enum"].push_back(item);
    }
}

void from_json(const nlohmann::json& object, property& property)
{
    object.front().get_to(property.name);
    object.front().at("type").get_to(property.type);
    object.front().at("description").get_to(property.description);

    if(object.front().contains("enumeration"))
    {
        object.front().at("enum").get_to(property.enumeration);
    }
}

tool::tool(const std::string& name, const std::string& description) : m_name(name), m_description(description)
{
}

tool::~tool()
{
}

std::string tool::get_name() const
{
    return m_name;
}

nlohmann::json tool::format() const
{
    nlohmann::json output = {
        {"type", "function"},
        {"name", m_name},
        {"description", m_description},
        {"strict", m_strict},
        {"parameters",
         {{"type", "object"},
          {"additionalProperties", m_additional_properties},
          {"properties", nlohmann::json::object()}}}};

    for(auto& property: m_properties)
    {
        output.at("parameters").at("properties")[property.name] = property;
        output.at("parameters")["required"].push_back(property.name);
    }

    return output;
}