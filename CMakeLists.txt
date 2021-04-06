cmake_minimum_required(VERSION 3.16)
project(HeyMorty C)

set(CMAKE_C_STANDARD 99)

find_library(curl curl)
find_library(flac flac)
find_library(asound asound)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wdouble-promotion -Wformat=2 -Wformat-truncation -Wformat-overflow -Wundef -fno-common -Wconversion")
endif ()

file(GLOB_RECURSE SRC "src/*.c" "src/*.h")

add_executable(HeyMorty
        ${SRC})

target_link_libraries(HeyMorty curl)
target_link_libraries(HeyMorty FLAC)
target_link_libraries(HeyMorty asound)
target_link_libraries(HeyMorty Threads::Threads)
