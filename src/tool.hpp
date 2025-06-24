#pragma once

#include <concepts>
#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

struct property
{
    std::string              name;
    std::string              description;
    std::vector<std::string> enumeration = {};
    std::string              type = "string";
    bool                     required = true;
};

void to_json(nlohmann::json& object, const property& property);
void from_json(const nlohmann::json& object, property& property);

class tool
{
public:
    tool(const std::string& name, const std::string& description);
    virtual ~tool() = 0;

    nlohmann::json format() const;
    std::string    get_name() const;

    virtual std::string callback(const std::map<std::string, std::string>& arguments) = 0;

protected:
    const std::string     m_name;
    const std::string     m_description;
    std::vector<property> m_properties;
    const bool            m_strict = false;
    const bool            m_additional_properties = false;
};