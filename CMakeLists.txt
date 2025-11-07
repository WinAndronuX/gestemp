cmake_minimum_required(VERSION 3.10)

project(gestemp C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(listview STATIC
    lib/listview/src/listview.c
)

target_include_directories(listview
    PUBLIC
        lib/listview/include
)

add_executable(gestemp
    src/main.c
    src/fan.c
    src/tempsensor.c
    src/utils.c
    src/zone.c
    src/users.c
    src/menu.c
    src/tempctrl.c
)

target_include_directories(gestemp
    PRIVATE
        include
)

target_link_libraries(gestemp PRIVATE listview)

configure_file(config.ini config.ini COPYONLY)