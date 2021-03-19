cmake_minimum_required(VERSION 3.17)
project(HeyMorty C)

set(CMAKE_C_STANDARD 99)

find_library(curl curl)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wdouble-promotion -Wformat=2 -Wformat-truncation -Wformat-overflow -Wundef -fno-common -Wconversion")
endif ()

add_executable(HeyMorty src/main.c)

target_link_libraries(HeyMorty curl)
