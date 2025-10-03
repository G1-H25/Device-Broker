# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.cpp)
FILE(GLOB_RECURSE app_includes ${CMAKE_SOURCE_DIR}/include/**/*.*)

idf_component_register(SRCS ${app_sources} ${app_includes}
                    REQUIRES spi_flash unity arduinojson )
