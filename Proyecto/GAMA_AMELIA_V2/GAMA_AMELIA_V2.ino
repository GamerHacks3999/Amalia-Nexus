// ============================================================
//  GAMA_AMELIA_V2 — Control Dual de Servos vía Bluetooth
//  Ejes X (Izquierda/Derecha) y Y (Arriba/Abajo)
//  Comandos: I=Izq, D=Der, A=Arriba, B=Abajo, S=Stop
//  Combinados: Q=Arr-Izq, E=Arr-Der, Z=Abj-Izq, C=Abj-Der
// ============================================================

#include "BluetoothSerial.h"
#include <ESP32Servo.h>

// --- INSTANCIAS ---
BluetoothSerial SerialBT;
Servo servoX;
Servo servoY;

// --- PINES ---
const int PIN_SERVO_X = 14;
const int PIN_SERVO_Y = 12;

// --- LÍMITES Y VELOCIDAD ---
const int ANGULO_CENTRO = 90;
const int ANGULO_MIN    = 0;
const int ANGULO_MAX    = 180;

// Velocidad balanceada: 2° por ciclo a 15ms = ~4.5s de extremo a extremo
const int  PASO_GIRO      = 2;
const int  PASO_RETORNO   = 1;
const int  DELAY_CICLO    = 15;   // ms por iteración del loop
const int  DELAY_RETORNO  = 12;   // ms entre pasos del retorno suave

// --- TEMPORIZADOR DE INACTIVIDAD ---
const unsigned long TIEMPO_RETORNO_MS = 10000; // 10 s sin movimiento → centrar
unsigned long tiempoUltimoMovimiento  = 0;

// --- ESTADO GLOBAL ---
int  anguloX        = ANGULO_CENTRO;
int  anguloY        = ANGULO_CENTRO;
char comandoActual  = 'S';
bool clienteConectado = false;

// ============================================================
//  FUNCIÓN: moverEje
//  Mueve un servo un paso en la dirección indicada.
//  delta: +PASO_GIRO o -PASO_GIRO
//  Devuelve true si hubo movimiento real.
// ============================================================
bool moverEje(Servo& servo, int pin, int& angulo, int delta) {
  int nuevo = constrain(angulo + delta, ANGULO_MIN, ANGULO_MAX);
  if (nuevo == angulo) return false;          // ya en el límite

  if (!servo.attached()) {
    servo.setPeriodHertz(50);
    servo.attach(pin, 500, 2400);
  }
  angulo = nuevo;
  servo.write(angulo);
  return true;
}

// ============================================================
//  FUNCIÓN: retornarAlCentro
//  Lleva ambos ejes suavemente a 90° y apaga los motores.
// ============================================================
void retornarAlCentro() {
  if (anguloX == ANGULO_CENTRO && anguloY == ANGULO_CENTRO) return;

  Serial.println("[Protocolo] Retornando ambos ejes al centro...");

  if (!servoX.attached()) { servoX.setPeriodHertz(50); servoX.attach(PIN_SERVO_X, 500, 2400); }
  if (!servoY.attached()) { servoY.setPeriodHertz(50); servoY.attach(PIN_SERVO_Y, 500, 2400); }

  while (anguloX != ANGULO_CENTRO || anguloY != ANGULO_CENTRO) {
    if      (anguloX > ANGULO_CENTRO) anguloX -= PASO_RETORNO;
    else if (anguloX < ANGULO_CENTRO) anguloX += PASO_RETORNO;

    if      (anguloY > ANGULO_CENTRO) anguloY -= PASO_RETORNO;
    else if (anguloY < ANGULO_CENTRO) anguloY += PASO_RETORNO;

    servoX.write(anguloX);
    servoY.write(anguloY);
    delay(DELAY_RETORNO);
  }

  delay(150);
  servoX.detach();
  servoY.detach();

  Serial.println("[OK] Centro asegurado. Motores apagados.");
  Serial.printf("     X=%d°  Y=%d°\n", anguloX, anguloY);

  comandoActual        = 'S';
  tiempoUltimoMovimiento = millis();
}

// ============================================================
//  SETUP
// ============================================================
void setup() {
  Serial.begin(115200);
  SerialBT.begin("GAMA_AMELIA_V2");

  // Posición inicial: centro
  servoX.setPeriodHertz(50);
  servoX.attach(PIN_SERVO_X, 500, 2400);
  servoX.write(anguloX);

  servoY.setPeriodHertz(50);
  servoY.attach(PIN_SERVO_Y, 500, 2400);
  servoY.write(anguloY);

  delay(600);
  servoX.detach();
  servoY.detach();

  Serial.println("======================================");
  Serial.println("  GAMA_AMELIA_V2 lista.");
  Serial.println("  Comandos: I D A B S | Q E Z C");
  Serial.println("======================================");
}

// ============================================================
//  LOOP PRINCIPAL
// ============================================================
void loop() {

  // ── 1. FAILSAFE DE CONEXIÓN ──────────────────────────────
  bool conexionActual = SerialBT.hasClient();
  if (!conexionActual && clienteConectado) {
    Serial.println("[ALERTA] Conexión perdida. Centrando ejes...");
    retornarAlCentro();
  }
  clienteConectado = conexionActual;

  // ── 2. LECTURA DE COMANDOS ───────────────────────────────
  if (SerialBT.available()) {
    char c = (char)SerialBT.read();

    // Comandos válidos (ejes individuales + diagonales + stop)
    const char* validos = "IDABSQEZCidasqezc";
    bool esValido = false;
    for (int i = 0; validos[i] != '\0'; i++) {
      if (c == validos[i]) { esValido = true; break; }
    }

    if (esValido) {
      comandoActual = toupper(c);          // normalizar a mayúscula
      if (comandoActual != 'S') {
        tiempoUltimoMovimiento = millis();
      }
      Serial.printf("[CMD] %c  |  X=%d°  Y=%d°\n", comandoActual, anguloX, anguloY);
    }
  }

  // ── 3. EJECUCIÓN DE MOVIMIENTO ───────────────────────────
  if (clienteConectado) {

    bool movio = false;

    switch (comandoActual) {

      // --- Eje X ---
      case 'I': movio = moverEje(servoX, PIN_SERVO_X, anguloX, -PASO_GIRO); break;
      case 'D': movio = moverEje(servoX, PIN_SERVO_X, anguloX, +PASO_GIRO); break;

      // --- Eje Y ---
      case 'A': movio = moverEje(servoY, PIN_SERVO_Y, anguloY, +PASO_GIRO); break;
      case 'B': movio = moverEje(servoY, PIN_SERVO_Y, anguloY, -PASO_GIRO); break;

      // --- Diagonales (ambos ejes simultáneos) ---
      case 'Q': // Arriba-Izquierda
        movio  = moverEje(servoX, PIN_SERVO_X, anguloX, -PASO_GIRO);
        movio |= moverEje(servoY, PIN_SERVO_Y, anguloY, +PASO_GIRO);
        break;
      case 'E': // Arriba-Derecha
        movio  = moverEje(servoX, PIN_SERVO_X, anguloX, +PASO_GIRO);
        movio |= moverEje(servoY, PIN_SERVO_Y, anguloY, +PASO_GIRO);
        break;
      case 'Z': // Abajo-Izquierda
        movio  = moverEje(servoX, PIN_SERVO_X, anguloX, -PASO_GIRO);
        movio |= moverEje(servoY, PIN_SERVO_Y, anguloY, -PASO_GIRO);
        break;
      case 'C': // Abajo-Derecha
        movio  = moverEje(servoX, PIN_SERVO_X, anguloX, +PASO_GIRO);
        movio |= moverEje(servoY, PIN_SERVO_Y, anguloY, -PASO_GIRO);
        break;

      // --- Stop + auto-centrado por inactividad ---
      case 'S':
        if (servoX.attached()) servoX.detach();
        if (servoY.attached()) servoY.detach();

        if ((anguloX != ANGULO_CENTRO || anguloY != ANGULO_CENTRO) &&
            (millis() - tiempoUltimoMovimiento >= TIEMPO_RETORNO_MS)) {
          Serial.println("[Auto] 10 s de inactividad. Centrando...");
          retornarAlCentro();
        }
        break;
    }

    // Actualizar timestamp si hubo movimiento real
    if (movio) tiempoUltimoMovimiento = millis();
  }

  delay(DELAY_CICLO);
}
