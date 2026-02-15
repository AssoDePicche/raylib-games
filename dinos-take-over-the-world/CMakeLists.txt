cmake_minimum_required(VERSION 3.16)

project(App LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")

add_subdirectory(main)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/bin)
