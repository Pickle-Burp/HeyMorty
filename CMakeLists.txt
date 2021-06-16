cmake_minimum_required(VERSION 3.16)
project(HeyMorty C)

set(CMAKE_C_STANDARD 99)

FIND_PACKAGE(PkgConfig REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_library(curl curl)
find_library(tensorflow libtensorflow-bin)
find_library(json-c json-c)
find_library(gtk3 gtk+-3.0)
pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})

link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})


if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wdouble-promotion -Wformat=2 -Wformat-truncation -Wformat-overflow -Wundef -fno-common -Wconversion")
endif ()

add_executable(HeyMorty
        src/main.c
        src/cmd/execution.h
        src/cmd/execution.c
        src/cmd/spell_check.h
        src/cmd/spell_check.c
        src/neural_network/layer.h
        src/neural_network/layer.c
        src/neural_network/neuron.h
        src/neural_network/neuron.c
        src/neural_network/nn.h
        src/neural_network/nn.c
        src/sprec/sprec.h
        src/sprec/recognize.h
        src/sprec/recognize.c
        src/sprec/web_client.h
        src/sprec/web_client.c
        src/utils/file.h
        src/utils/file.c
        src/utils/string_builder.h
        src/utils/string_builder.c
        src/ui/ui.h
        src/ui/ui.c
        src/neural_network/cnn.c
        src/neural_network/cnn.h)

target_link_libraries(HeyMorty curl)
target_link_libraries(HeyMorty Threads::Threads)
target_link_libraries(HeyMorty m)
target_link_libraries(HeyMorty tensorflow)
target_link_libraries(HeyMorty json-c)
target_link_libraries(HeyMorty PkgConfig::GLIB)
target_link_libraries(HeyMorty ${GTK3_LIBRARIES})