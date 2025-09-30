FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/main/**/*.cpp)
FILE(GLOB_RECURSE include_sources ${CMAKE_SOURCE_DIR}/include/**/*.*)

set(include_dir ${CMAKE_SOURCE_DIR}/include)

idf_component_register(SRCS src/main.cpp ${include_sources}
                    INCLUDE_DIRS "." ${include_dir}
                    REQUIRES esp_http_client nvs_flash unity spi_flash)
