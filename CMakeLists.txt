cmake_minimum_required(VERSION 3.15)
project(tokaido_2310a C)

set(CMAKE_C_STANDARD 99)

add_executable(tokaido_2310a 2310dealer.c error.c error.h helper.c helper.h player.c player.h path.c path.h game.c game.h)