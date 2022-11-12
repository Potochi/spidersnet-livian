cmake_minimum_required(VERSION 3.24)


project(SuperFalcon C)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

include_directories("./gbdk/include/")

add_executable(chall.gba main.c tileset.c tilemap.c loading.c Export.c)
