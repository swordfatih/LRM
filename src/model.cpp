#include "model.hpp"

#include <cpr/cpr.h>

model::model() : m_base_url("https://api.openai.com/v1/"), m_api_key(environment::get().at("api_key")), m_model("gpt-3.5-turbo"), m_temperature(0.7), m_max_tokens(500)
{
}

model::~model()
{
}

std::string model::query(const context& context)
{
    nlohmann::json request_body = {
        {"model", m_model},
        {"messages", context.format()},
        {"max_tokens", m_max_tokens},
        {"temperature", m_temperature}};

    cpr::Response r = cpr::Post(cpr::Url{m_base_url + "chat/completions"},
                                cpr::Header{{"Authorization", "Bearer " + m_api_key},
                                            {"Content-Type", "application/json"}},
                                cpr::Body{request_body.dump()},
                                cpr::Timeout{10000});

    spdlog::debug("(model.cpp) response:\n{}", r.text);

    nlohmann::json response_json = nlohmann::json::parse(r.text);
    std::string    reply = response_json["choices"][0]["message"]["content"];

    return reply;
}
