#pragma once

#include <nlohmann/json.hpp>
#include <tao/pq.hpp>

class database
{
public:
    database();
    ~database();

    std::optional<std::string> execute(const std::string& query);

private:
    std::string format(const tao::pq::result& result);

    std::shared_ptr<tao::pq::connection> m_connection;
};