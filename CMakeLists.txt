cmake_minimum_required(VERSION 3.16)
project(HeyMorty C)

set(CMAKE_C_STANDARD 99)

find_library(curl curl)
find_library(flac flac)
find_library(asound asound)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wdouble-promotion -Wformat=2 -Wformat-truncation -Wformat-overflow -Wundef -fno-common -Wconversion")
endif ()

add_executable(HeyMorty src/main.c src/sprec/web_client.c src/sprec/web_client.h src/sprec/flac_encoder.c src/sprec/flac_encoder.h src/sprec/wav.c src/sprec/wav.h)

target_link_libraries(HeyMorty curl)
target_link_libraries(HeyMorty FLAC)
target_link_libraries(HeyMorty asound)
