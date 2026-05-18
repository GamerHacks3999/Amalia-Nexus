# 🚀 Amalia-Nexus: Sistema de Monitoreo, Conectividad y Control Robótico

Proyecto escolar integral de alta fidelidad que combina visión por computadora, enrutamiento de red local y control de actuadores mediante una arquitectura distribuida de tres microcontroladores ESP32 y una aplicación dedicada.

---

## 💻 Arquitectura del Sistema (Ecosistema de 3 Nodos)

Para maximizar el rendimiento y evitar cuellos de botella, el sistema divide sus tareas en tres hardware independientes que trabajan en paralelo:

### 1. Nodo Módem / Repetidor NAT (ESP32-WROOM)
*   **Función:** Actúa como punto de acceso (AP) seguro y repetidor con traducción de direcciones de red (NAPT). Provee la infraestructura de red para el resto de los componentes.
*   **Optimización:** Configurado con *Modem Sleep* y Dynamic Frequency Scaling (DFS) a 80MHz para operar eficientemente con baterías de 3.7V.

### 2. Nodo de Transmisión de Video (ESP32-S3 Cam)
*   **Función:** Procesamiento de imágenes y transmisión de video por streaming en tiempo real hacia la interfaz web a través de la red generada por el módem.

### 3. Nodo de Control de Actuadores (ESP32 Standard)
*   **Función:** Gestión y posicionamiento de los servomotores para el movimiento físico del sistema.
*   **Interfaz de Control:** Se comunica de forma nativa con la **Aplicación Móvil Dedicada** desarrollada para este proyecto.

---

## 📁 Estructura del Repositorio

El repositorio está organizado para mantener separados los entornos de desarrollo de cada placa y el software de control:

*   `proyecto/`
    *   📁 `ESP32_Modem_NAT/` -> Código fuente del módem repetidor de bajo consumo.
    *   📁 `ESP32_S3_Streaming/` -> Código fuente para la captura y transmisión de video web.
    *   📁 `ESP32_Control_Servos/` -> Código fuente para la recepción de comandos y movimiento de servos.
    *   📁 `App_Control_Amalia/` -> **¡Importante!** Aquí se encuentra el software/código fuente de la aplicación móvil diseñada para controlar los servos.

---

## 🛠️ Requisitos de Instalación (Guía para el Profesor)

### Para los Microcontroladores (IDE de Arduino):
1. **Gestor de Tarjetas ESP32:** Asegúrese de tener instalado el core de ESP32 de Espressif (v2.0.x o superior) en el IDE.
2. **Librerías Requeridas:**
   * `Adafruit_SSD1306` y `Adafruit_GFX` (Para el despliegue de estatus físico).
   * Librerías nativas de la arquitectura ESP32-S3 para el manejo del sensor de cámara.

### Para la Aplicación Móvil:
* Revise la carpeta `App_Control_Amalia/` para compilar el código fuente o instalar directamente el ejecutable proporcionado para interactuar con el ESP32 de los servos.

---

## 🔒 Parámetros de Red y Seguridad por Defecto

*   **SSID del Punto de Acceso:** `GAMA_Nexus`
*   **Contraseña AP:** `GAMA_2026_SEC`
*   **Ruta del Panel del Módem:** `/panel_escom_77`
*   **Credenciales de Acceso Base:**
    *   *Usuario:* `Angel_Admin`
    *   *Contraseña:* `Proto_ESCOM_0x44`

---
⚙️ *Proyecto desarrollado con fines académicos siguiendo los lineamientos técnicos de la guía proporcionada.*
