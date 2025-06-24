#pragma once

#include <nlohmann/json.hpp>
#include <tao/pq.hpp>

class database
{
public:
    database();
    ~database() = default;

    std::optional<std::string> execute(const std::string& query);
    
    static bool                is_valid_identifier(const std::string& ident);

private:
    std::string format(const tao::pq::result& result);

    std::shared_ptr<tao::pq::connection> m_connection;
};