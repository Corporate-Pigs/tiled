cmake_minimum_required(VERSION 3.26.4)

project(tiled)
add_compile_options(-Wall -Wextra)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
add_library(tiled STATIC ${SOURCES})

add_subdirectory(vendor/json)

target_include_directories(tiled PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(tiled PUBLIC nlohmann_json)
