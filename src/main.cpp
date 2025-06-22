#include "language.hpp"
#include <iostream>

int main()
{
    spdlog::set_level(spdlog::level::debug);

    language                   language;
    std::optional<std::string> result;

    language.query("list all the user created tables and their columns");
    language.query("insert a new book");
    language.query("add a color column to books if not exists");
    language.query("update the color of books to blue");
    language.query("show all books");

    return EXIT_SUCCESS;
}