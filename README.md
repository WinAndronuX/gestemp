# GESTEMP - Sistema de Gestión de Temperatura Simulada

![Lenguaje](https://img.shields.io/badge/language-C11-blue.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)
![Licencia](https://img.shields.io/badge/license-MIT-blue.svg)

**GESTEMP** es un sistema de simulación de control de temperatura basado en consola, escrito en C modular. Permite gestionar zonas, simular cambios de temperatura en tiempo real y registrar un historial de eventos. Es multiplataforma y funciona tanto en Linux como en Windows.

## Características Principales

El sistema se organiza en un menú interactivo con las siguientes capacidades:

* **Gestión de Zonas:** Crear, visualizar, modificar y eliminar zonas de monitoreo.
* **Control de Temperatura:**
    * **Simulación Real:** Un hilo dedicado actualiza la temperatura de las zonas según una fórmula física y la configuración.
    * **Control Manual/Automático:** Permite forzar el encendido/apagado de ventiladores o dejarlos en modo automático basado en umbrales.
    * **Monitor en Vivo:** Una pantalla que se refresca en tiempo real mostrando el estado de todas las zonas.
* **Consultas e Historial:**
    * **Registro de Eventos:** Guarda cada cambio (manual o automático) en `historial.log`.
    * **Resumen estadístico:** Muestra análisis estadítico de las temperaturas para poder analizar sus resultados.
    * **Exportación a CSV:** Genera un reporte `.csv` con filtros personalizables.
* **Gestión de Usuarios:** Sistema de login con roles (Administrador, Operador, Visitante) para controlar el acceso a funciones.
* **Configuración:**
    * Ajusta los umbrales de temperatura por zona.
    * Restaura la configuración por defecto de una zona.

## Cómo Empezar

### Prerrequisitos

* **CMake** (Versión 3.10 o superior)
* Un compilador de C (como **GCC** en Linux o **MinGW** en Windows)
* **Git**

### Pasos para Compilar

1.  **Clona el repositorio:**
    ```bash
    git clone https://github.com/WinAndronuX/gestemp.git
    cd gestemp
    ```

2.  **Inicializa el submódulo `listview`:**
    ```bash
    git submodule update --init --recursive
    ```

3.  **Crea y entra al directorio de compilación:**
    ```bash
    mkdir build
    cd build
    ```

4.  **Configura el proyecto con CMake:**
    ```bash
    cmake ..
    ```

5.  **Compila el proyecto:**
    ```bash
    cmake --build .
    ```

### Ejecutar el Programa

El ejecutable (`gestemp.exe` o `gestemp`) estará dentro de la carpeta `build/`.

```bash
# En Windows
./gestemp.exe

# En Linux o Mac
./gestemp
```

### Configuración

La simulación se controla mediante config.ini, que debe estar en la misma carpeta que el ejecutable.

  * **AIR_DENSITY:** Densidad del aire 
  * **AIR_HEAT_CAPACITY:** Capacidad calorífica del aire 
  * **EXTERN_TEMP:** Temperatura exterior de referencia
  * **MAX_TEMP / MIN_TEMP:** Límites superior e inferior de la simulación
