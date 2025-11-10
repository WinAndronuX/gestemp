# Sistema de Gestión de Temperatura Simulada

![Licencia](https://img.shields.io/badge/license-MIT-blue.svg)
![Estado del Build](https://img.shields.io/badge/build-passing-brightgreen.svg)
![Lenguaje](https://img.shields.io/badge/language-C11-blue.svg)

Un sistema de simulación de control de temperatura basado en consola, escrito en C modular. Permite gestionar zonas, simular cambios de temperatura en tiempo real y registrar un historial de eventos, multiplataforma funcionando en Linux y Windows. 

##  Características

- ### Zonas

**Ver Zonas:** Permite visualizar todas las zonas registradas y sus datos.

**Agregar zonas**: El usuario puede registrar nuevas zonas.

**Modificar zona**: Con esta función podemos modificar los datos de las zonas.



- ### Control De Temperatura

 **Simulación en Tiempo Real:** Un hilo separado actualiza el estado de la temperatura cada segundo.
 
 **Control Manual y Automático:** Los ventiladores se activan automáticamente por el hilo de simulación o manualmente por el usuario.
 
 **Monitor en Tiempo Real:** Una pantalla que se refresca cada segundo para mostrar el estado actual de todas las zonas.
 
 **Registro de Historial:** Todos los eventos (manuales y automáticos) se guardan en 'historial.log'.



- ### Consultas

**Buscar eventos por rango de temperatura o fecha**: El usuario puede filtrar los eventos por fecha o temperatura para visualizar el historial.

**Exportación a CSV:** Permite exportar a un archivo '.csv'

**Función Estadistica:** Ayuda a poder analizar los datos de manera concisa, utilizando análisis estadístico de las zonas.


- ### Usuarios

**Ver Usuarios:** Permite mostrar una lista de todos los usuarios registrados

**Agregar Usuario**: Un administrador puede agregar nuevos usuarios al sistema, definiendo su nombre, contraseña y rol.

**Borrar Usuario:** Un administrador puede eliminar un usuario existente.

**Cambiar contraseña**: Un administrador puede cambiar la contraseña de un usuario existente.

- ### Configuración

**Configurar umbral de temperatura por zona:** Nos sirve para la configuración de el umbral de alguna zona especifica. 

**Restaurar configuración por defecto por zona:** Regresa los cambios hechos a una zona a un valor por defecto y elimina el historial asociado.

---
##  Cómo Empezar

Pasos para compilar y ejecutar el proyecto.

### Prerrequisitos

* **CMake** (Versión 3.10 o superior)
* Un compilador de C (como **GCC** en Linux o **MinGW** en Windows)
* **Git**

### Pasos para Compilar

1.  **Clona (descarga) el repositorio:**
    ```bash
    git clone [https://github.com/WinAndronuX/gestemp.git](https://github.com/WinAndronuX/gestemp.git)
    cd gestemp
    ```

2.  **Inicializa la librería 'listview' (Submódulo):**
    ```bash
    git submodule update --init --recursive
    ```

3.  **Crea la carpeta de compilación:**
    ```bash
    mkdir build
    cd build
    ```

4.  **Prepara la compilación con CMake:**
    ```bash
    cmake ..
    ```

5.  **Compila el proyecto:**
    ```bash
    cmake --build .
    ```

### Ejecutar el Programa

El ejecutable ('gestemp.exe' o 'gestemp') estará dentro de la carpeta 'build/'.

```bash
# En Windows (a veces está en 'build/Debug/')
./gestemp.exe

# En Linux o Mac
./gestemp
```

---

##  Configuración

La simulación se controla mediante 'config.ini', que debe estar en la misma carpeta que el ejecutable.

- **'AIR_DENSITY'**: Densidad del aire (ej. '1.12')

- **'AIR_HEAT_CAPACITY'**: Capacidad calorífica del aire (ej. '1005')

- **'EXTERN_TEMP'**: Temperatura exterior de referencia

- **'MAX_TEMP' / 'MIN_TEMP'**: Límites de temperatura de la simulación


---

##  Estructura del Proyecto

El proyecto está organizado de forma modular para separar responsabilidades.

- '/include/gestemp/' (Contiene los archivos '.h' o "anuncios" públicos)

- '/src/' (Contiene los archivos '.c' o "lógica" privada)

- '/lib/listview/' (Librería externa para dibujar tablas)

- 'CMakeLists.txt' (El "plano" para construir todo)

- 'README.md' (Este archivo)


### Descripción de Módulos Clave ('src/')

- **'main.c'**: Inicia el programa, maneja el menú principal y lanza el hilo de simulación.

- **'zone.c'**: Se encarga de guardar, cargar y simular las zonas.

- **'users.c'**: Maneja el login y el archivo de usuarios.

- **'tempctrl.c'**: Contiene toda la lógica de pantalla para el menú "Control de Temperaturas".

- **'queries.c'**: Se encarga de leer el historial y exportar a CSV.

- **'tempsensor.c'**: Provee la fórmula matemática para calcular la nueva temperatura.

- **'utils.c'**: Funciones de ayuda (limpiar pantalla, esperar) que funcionan en Windows y Linux.

- **'menu.c'**: Almacena el texto de los menús y la función de entrada.


---

##  Licencia

Distribuido bajo la Licencia MIT. Consulta el archivo 'LICENSE' para más detalles.
