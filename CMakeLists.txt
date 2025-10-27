cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(gestemp C)

# Establecemos el estándar de C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Incluimos cabeceras
include_directories(include)

# Archivos fuente
set(SOURCES
        src/main.c
        src/fan.c
        src/tempsensor.c
        src/utils.c
        src/zone.c
)

# Creamos el ejecutable
add_executable(gestemp ${SOURCES})
