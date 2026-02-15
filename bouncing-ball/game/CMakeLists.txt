add_library(game STATIC
  ball.cpp
)

find_package(raylib REQUIRED)

target_link_libraries(game PRIVATE raylib)

target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
