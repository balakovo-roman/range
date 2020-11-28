cmake_minimum_required(VERSION 3.8)

project(ranges)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Add source to this project's executable.
add_executable (ip_filter "ip_filter.cpp")

set_target_properties(ip_filter PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        )

find_package(range-v3 CONFIG REQUIRED)
# Note: 2 target(s) were omitted.
target_link_libraries(ip_filter PRIVATE range-v3 range-v3-meta range-v3::meta range-v3-concepts)
