#include "responses.hpp"
#include "tools/execute.hpp"
#include "tools/list.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

int main()
{
    // spdlog::set_level(spdlog::level::debug);

    responses responses;
    responses.use<tools::execute>();
    responses.use<tools::list>();

    std::string input;
    while(std::getline(std::cin, input))
    {
        std::optional<std::string>            id;
        std::vector<nlohmann::json::object_t> context{{{"content", input},
                                                       {"role", "user"},
                                                       {"type", "message"}}};

        auto output = responses.create(context, id);

        spdlog::info("output: {}", output.dump());
    }

    // responses.create("create a book table if it doesn't exist");
    // responses.create("insert a new book");
    // responses.create("add a color column to books if not exists");
    // responses.create("update the color of books to blue");
    // responses.create("show all books");

    return EXIT_SUCCESS;
}