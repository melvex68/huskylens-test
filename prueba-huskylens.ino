#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

HUSKYLENS huskylens;
SoftwareSerial mySerial(10, 11); // RX, TX
//HUSKYLENS cable verde >> Pin 10; cable azul >> Pin 11
void printResult(HUSKYLENSResult result);

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);
    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("¡Ha fallado!"));
        Serial.println(F("1.Por favor comprueba que en \"Protocol Type\" de la HUSKYLENS tiene (General Settings>>Protocol Type>>Serial 9600)"));
        Serial.println(F("2.Comprueba las conexiones."));
        delay(100);
    }
}

void loop() {
    if (!huskylens.request()) Serial.println(F("Fallo al pedir datos a la HUSKYLENS, compueba las conexiones."));
    else if(!huskylens.isLearned()) Serial.println(F("Nada que aprender, pulsa el botón de aprendizaje en la HUSKYLENS para aprender uno"));
    else if(!huskylens.available()) Serial.println(F("No aparecen bloques o flechas en la pantalla."));
    else
    {
        Serial.println(F("###########"));
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);
        }    
    }
}

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Bloque:xCentro=")+result.xCenter+F(",yCentro=")+result.yCenter+F(",ancho=")+result.width+F(",alto=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigen=")+result.xOrigin+F(",yOrigen=")+result.yOrigin+F(",xDestino=")+result.xTarget+F(",yDestino=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Objeto desconocido");
    }
}
