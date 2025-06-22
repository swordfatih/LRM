#pragma once

#include <fstream>
#include <map>
#include <string>

using env_vars = std::map<std::string, std::string>;

class environment
{
public:
    static const env_vars& get();
    void                   load(const std::string& path);

private:
    environment();

    env_vars m_vars;
};