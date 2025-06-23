#include "responses.hpp"
#include "environment.hpp"
#include <cpr/cpr.h>
#include <format>
#include <spdlog/spdlog.h>

responses::responses() : m_api_key(environment::get().at("api_key")),
                         m_base_url("https://api.openai.com/v1/"),
                         m_model("gpt-4.1-nano"),
                         m_temperature(1.f)
{
}

responses::~responses()
{
}

nlohmann::json responses::post(const std::string& endpoint, const nlohmann::json& body)
{
    cpr::Response r = cpr::Post(cpr::Url{m_base_url + endpoint},
                                cpr::Header{{"Authorization", std::format("Bearer {}", m_api_key)},
                                            {"Content-Type", "application/json"}},
                                cpr::Body{body.dump()});

    auto response = nlohmann::json::parse(r.text);

    spdlog::debug("(responses.cpp:post) parsed:\n{}", response.dump());

    return response;
}

void responses::create(const std::string& input)
{
    nlohmann::json body = {{"model", m_model},
                           {"input", input}};

    if(!m_id.empty())
    {
        body["previous_response_id"] = m_id;
    }

    spdlog::debug("(responses.cpp:create) body:\n{}", body.dump());

    auto response = post("responses", body);

    if(response.contains("error") && response.at("error") != nullptr)
    {
        spdlog::error("(responses.cpp:create) api:\n{}", response.at("error").dump());
        return;
    }

    m_id = response.at("id");

    spdlog::debug("(responses.cpp:create) response:\n{}", response.at("output").dump());
}