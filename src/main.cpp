#include "language.hpp"
#include <iostream>

int main()
{
    spdlog::set_level(spdlog::level::debug);

    language                   language;
    std::optional<std::string> result;

    language.db_query("list all the user created tables and their columns");
    language.db_query("insert a new book");
    language.db_query("add a color column to books if not exists");
    language.db_query("update the color of books to blue");
    language.db_query("show all books");

    return EXIT_SUCCESS;
}