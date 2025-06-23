#include "responses.hpp"
#include <spdlog/spdlog.h>

int main()
{
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("before response");
    responses responses;
    spdlog::info("after response");

    responses.create("list all the user created tables and their columns");
    // responses.create("create a book table if it doesn't exist");
    // responses.create("insert a new book");
    // responses.create("add a color column to books if not exists");
    // responses.create("update the color of books to blue");
    // responses.create("show all books");

    return EXIT_SUCCESS;
}