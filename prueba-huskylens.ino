//
// Información sobre la placa HuskyLens y el código en:
// https://wiki.dfrobot.com/HUSKYLENS_V1.0_SKU_SEN0305_SEN0336
//

#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

// Define huskylens como una instancia de la clase HUSKYLENS para interactuar con la placa.
HUSKYLENS huskylens;

// Configura una conexión serial de software en los pines 10 y 11.
SoftwareSerial mySerial(10, 11); // Pin 10 conectado a RX de HUSKYLENS, Pin 11 a TX.

// Prototipo de la función que imprime los resultados de HuskyLens.
void printResult(HUSKYLENSResult result);

void setup() {
    // Inicia la comunicación serial con la computadora.
    Serial.begin(115200);
    // Inicia la comunicación serial con HuskyLens.
    mySerial.begin(9600);

    // Intenta iniciar la comunicación con HuskyLens hasta que tenga éxito.
    while (!huskylens.begin(mySerial)) {
        Serial.println(F("¡Ha fallado!"));
        Serial.println(F("1. Asegúrate de que la HUSKYLENS esté configurada en: (General Settings>>Protocol Type>>Serial 9600)"));
        Serial.println(F("2. Comprueba las conexiones de nuevo."));
        delay(100);
    }
}

void loop() {
    // Solicita datos a HuskyLens.
    if (!huskylens.request()) {
        Serial.println(F("Fallo al pedir datos a la HUSKYLENS, comprueba las conexiones."));
    } else {
        // Procesa los datos recibidos si hay algo para aprender o reconocer.
        if (!huskylens.isLearned()) {
            Serial.println(F("Nada que aprender, pulsa el botón de aprendizaje en la HUSKYLENS."));
        } else if (!huskylens.available()) {
            Serial.println(F("No hay bloques ni flechas detectados."));
        } else {
            // Imprime los resultados mientras haya datos disponibles.
            Serial.println(F("###########"));
            while (huskylens.available()) {
                HUSKYLENSResult result = huskylens.read();
                printResult(result);
            }
        }
    }
}

// Imprime los detalles del objeto o flecha detectada por HuskyLens.
void printResult(HUSKYLENSResult result) {
    if (result.command == COMMAND_RETURN_BLOCK) {
        Serial.print(F("Bloque: xCentro="));
        Serial.print(result.xCenter);
        Serial.print(F(", yCentro="));
        Serial.print(result.yCenter);
        Serial.print(F(", Ancho="));
        Serial.print(result.width);
        Serial.print(F(", Alto="));
        Serial.print(result.height);
        Serial.print(F(", ID="));
        Serial.println(result.ID);
    } else if (result.command == COMMAND_RETURN_ARROW) {
        Serial.print(F("Flecha: xOrigen="));
        Serial.print(result.xOrigin);
        Serial.print(F(", yOrigen="));
        Serial.print(result.yOrigin);
        Serial.print(F(", xDestino="));
        Serial.print(result.xTarget);
        Serial.print(F(", yDestino="));
        Serial.print(result.yTarget);
        Serial.print(F(", ID="));
        Serial.println(result.ID);
    } else {
        Serial.println(F("Objeto desconocido."));
    }
}
