cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(gestemp C)

# Establecemos el estándar de C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# --- 1. Definición de la Librería (listview) ---

# Creamos un objetivo de librería llamado 'listview'
# STATIC significa que compilará como un archivo .a y se "pegará"
# dentro de tu ejecutable final.
add_library(listview STATIC
    lib/listview/src/listview.c
)

# Definimos los directorios de include para 'listview'
# 'PUBLIC' significa que cualquiera que se enlace con 'listview'
# (como tu app 'gestemp') también tendrá acceso a este directorio.
target_include_directories(listview
    PUBLIC
        lib/listview/include
)

# --- 2. Definición del Ejecutable (gestemp) ---

# Creamos el ejecutable (ya no necesitas la variable SOURCES)
add_executable(gestemp
    src/main.c
    src/fan.c
    src/tempsensor.c
    src/utils.c
    src/zone.c
    src/users.c
    src/menu.c
        src/tempctrl.cpp
        include/gestemp/tempctrl.h
)

# Le decimos a 'gestemp' dónde están SUS cabeceras
# 'PRIVATE' significa que esta ruta de include es solo para 'gestemp'
# (Esto reemplaza tu antiguo comando 'include_directories(include)')
target_include_directories(gestemp
    PRIVATE
        include
)

# --- 3. Enlazado (El paso clave) ---

# Le decimos al ejecutable 'gestemp' que depende de (se enlaza con)
# la librería 'listview'.
# CMake se encarga automáticamente de compilar 'listview' primero
# y de pasar la ruta de include (porque la pusimos como PUBLIC).
target_link_libraries(gestemp PRIVATE listview)

# --- 4. Copia de Archivos ---

# Esto sigue igual: copia el config.ini al directorio 'build'
configure_file(config.ini config.ini COPYONLY)