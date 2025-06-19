cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable C++20
add_executable(main)
target_compile_features(main PRIVATE cxx_std_20)

# Add sources
file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")
target_sources(main PRIVATE ${SOURCES})

# Fetch and include dependencies
include(FetchContent)

set(PostgreSQL_ROOT "C:/msys64/mingw64")
find_package(PostgreSQL REQUIRED)

FetchContent_Declare(
    taocpp-taopq
    GIT_REPOSITORY https://github.com/taocpp/taopq
    GIT_TAG main
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(taocpp-taopq)

FetchContent_Declare(
    CPR
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.11.3
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(CPR)

FetchContent_Declare(
    JSON
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(JSON)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.3
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(spdlog)

# Link libraries
target_link_libraries(main PRIVATE cpr::cpr nlohmann_json::nlohmann_json taocpp::taopq spdlog::spdlog)
