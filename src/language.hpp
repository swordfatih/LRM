#pragma once

#include "context.hpp"
#include "database.hpp"
#include "model.hpp"
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <string>

class language
{
public:
    language();
    ~language();

    void query(const std::string& request);

private:
    database m_database;
    model    m_model;
    context  m_context;
};