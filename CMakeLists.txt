cmake_minimum_required(VERSION 3.16)
project(CGI_C)

add_library(cgi_c STATIC
    src/platform.c
    # Add other source files if any
)

# Include headers
target_include_directories(cgi_c PUBLIC include)
# Set output directories
set_target_properties(cgi_c PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_NAME}"
)
