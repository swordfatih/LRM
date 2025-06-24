#include "environment.hpp"

environment::environment()
{
    load(".env");
}

env_vars& environment::get()
{
    static environment environment;
    return environment.m_vars;
}

void environment::load(const std::string& path)
{
    std::ifstream file(path);

    if(!file)
    {
        throw std::runtime_error("Error: Could not open .env file at " + path);
    }

    std::map<std::string, std::string> vars;
    std::string                        line;
    while(std::getline(file, line))
    {
        auto key = line.substr(0, line.find('='));
        auto value = line.substr(line.find('=') + 1);
        vars[key] = value;
    }

    m_vars = std::move(vars);
}