#pragma once

#include <vector>
#include <optional>
#include <string>
#include <spdlog/spdlog.h>

class context
{
public:
  context();
  ~context();

  void append(const std::optional<std::string>& item);
  void append(const std::string& item);

  std::string format() const;

private:
  std::vector<std::string> m_items;
};