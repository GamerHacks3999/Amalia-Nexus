# 🚀 Amalia-Nexus: Sistema de Monitoreo y Conectividad

Proyecto escolar integral que combina visión por computadora/captura de video y enrutamiento de red local utilizando microcontroladores ESP32. El sistema está diseñado para operar de forma conjunta, donde un nodo se encarga de la gestión de periféricos/video y el otro actúa como infraestructura de red de bajo consumo.

---

## 💻 Arquitectura del Proyecto

El sistema se divide en dos componentes principales que trabajan de la mano:

### 1. Nodo de Visión y Servos (ESP32-CAM-S3)
*   **Función:** Encargado de la captura de imágenes/video y del control de los servomotores para el movimiento del sistema (Paneo/Tildeo).
*   **Hardware:** Módulo ESP32-CAM-S3 con driver de servos integrado.

### 2. Nodo Módem / Repetidor NAT (ESP32-WROOM)
*   **Función:** Actúa como un punto de acceso (AP) seguro y repetidor con traducción de direcciones de red (NAPT). Permite el puenteo de internet hacia la ESP32-CAM optimizando el consumo energético.
*   **Hardware:** ESP32-WROOM-32D/U.
*   **Optimización Energética:** Configurado con políticas de *Modem Sleep* y Dynamic Frequency Scaling (DFS) a 80MHz para prolongar la vida útil operando con baterías LiPo/Li-ion de 3.7V.

---

## 📁 Estructura del Repositorio

Las carpetas están organizadas de la siguiente manera para su correcta compilación en el IDE de Arduino:

*   `proyecto/`
    *   📁 `ESP32_Modem_NAT/` -> Código fuente del módem repetidor optimizado para bajo consumo energético.
    *   📁 `ESP32_Cam_Servos/` -> Código fuente para la gestión de la cámara S3 y actuadores (servos).

---

## 🛠️ Requisitos de Instalación (Guía para el Profesor)

Para compilar y cargar estos códigos correctamente en el IDE de Arduino, asegúrese de contar con las siguientes dependencias:

1. **Gestor de Tarjetas ESP32:** 
   * Añadir la URL de Espressif en Preferencias: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   * Instalar la versión del core de ESP32 (se recomienda v2.0.x o v3.0.x según la compatibilidad de las librerías de la cámara S3).

2. **Librerías Necesarias:**
   * `Adafruit_SSD1306` y `Adafruit_GFX` (Para el despliegue de estatus en pantalla OLED si está equipada).
   * Librerías nativas de FreeRTOS y lwIP (incluidas en el core de ESP32 para el módem NAT).

---

## 🔒 Parámetros de Seguridad por Defecto

*   **SSID del Punto de Acceso:** `GAMA_Nexus`
*   **Contraseña AP:** `GAMA_2026_SEC`
*   **Ruta del Panel de Administración:** `/panel_escom_77`
*   **Credenciales de Acceso:**
    *   *Usuario:* `Angel_Admin`
    *   *Contraseña:* `Proto_ESCOM_0x44`

*Nota: El sistema cuenta con protección activa contra ataques de fuerza bruta (bloqueo temporal de IP por exceder intentos de inicio de sesión) y tokens de validación CSRF para mitigar acciones no autorizadas.*

---
⚙️ *Proyecto desarrollado con fines académicos siguiendo los lineamientos técnicos de la guía proporcionada.*
