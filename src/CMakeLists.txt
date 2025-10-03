# Check all files in /src and /include and add them to the project

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.cpp)

idf_component_register(SRCS ${app_sources} INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)
