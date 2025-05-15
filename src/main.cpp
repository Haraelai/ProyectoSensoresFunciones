#ANEXO ALEX PARA PRUEBA DE GIT
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
//Librerias para OTA
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "../lib/ota.h"
#include "../lib/interfazweb.h"
#include "../lib/reloj.h"

//Librerias para OTA

conexionWeb *webInterface;


DateTime *reloj;
StaticJsonDocument<512> horaActual;
StaticJsonDocument<512> post_ejemplo;
StaticJsonDocument<1024> get_ejemplo;
StaticJsonDocument<128> nivelesdegas;
StaticJsonDocument<128> consumoenergetico;
StaticJsonDocument<128> temperatura;
StaticJsonDocument<128> nivelesdeagua;
StaticJsonDocument<128> nivelesdeluz;
StaticJsonDocument<1024> alarmas;




const char *urlGas = "http://192.168.1.67/ProyectoSensores/public/api/fugasdegas";
const char *geturl = "http://192.168.100.155:8080/esp32-api/public/api/resetpassword";

void setup()
{
  Serial.begin(115200);
  setupOTA("TemplateSketch");

  webInterface = new conexionWeb(ssid, passwrd);
  reloj = new DateTime();

  pinMode(2, OUTPUT);
}
int sumatoria = 0;

void fugadegas() {
  String zonas [] = {"cocina", "sala", "patio", "baño", "recamara1", "recamara2"};
  String tipo [] = {"Tinaco1", "Tinaco2", "Sisterna casa", "Sisterna vieja","Tinaco 3", "Tanque"};
  String problems [] = {"fallo en la chimenea", "ventana sala abierta", "ventana recamara 1 abierta", "ventana recamara 2 abierta","Puerta principal abierta", "cochera abierta"};
  int x = random(300);
  nivelesdegas.clear();
  consumoenergetico.clear();
  temperatura.clear();
  nivelesdeagua.clear();
  nivelesdeluz.clear();
  int posicion = random(5);

  nivelesdegas["fechayhora"] = reloj->timeStringBuff;
  nivelesdegas["area"] = zonas [posicion];
  nivelesdegas["niveldegas"] = random(20 + posicion,25 + posicion);

  consumoenergetico["fechayhora"] = reloj->timeStringBuff;
  consumoenergetico["consumo"] = x;
  consumoenergetico["area"] = zonas [posicion];

  temperatura["fechayhora"] = reloj->timeStringBuff;
  temperatura["temperatura"] = random(10 + posicion,70 + posicion);
  temperatura["zona"] = zonas [posicion];

  nivelesdeagua["fechayhora"] = reloj->timeStringBuff;
  nivelesdeagua["tipocontenedor"] = tipo [posicion];
  nivelesdeagua["porcentaje"] = random(100);

  nivelesdeluz["fechayhora"] = reloj->timeStringBuff;
  nivelesdeluz["porcentaje"] = random(100);

  alarmas["fechayhora"] = reloj->timeStringBuff;
  alarmas["problema"] = problems [posicion];
  alarmas["area"] = "Casa principal";

  
  serializeJsonPretty(alarmas,Serial);
  webInterface->webPOST(alarmas, urlGas);
  serializeJsonPretty(nivelesdegas,Serial);
  webInterface->webPOST(nivelesdegas, urlGas);
  serializeJsonPretty(consumoenergetico,Serial);
  webInterface->webPOST(consumoenergetico, urlGas);
  serializeJsonPretty(temperatura,Serial);
  webInterface->webPOST(temperatura, urlGas);
    serializeJsonPretty(nivelesdeagua,Serial);
  webInterface->webPOST(nivelesdeagua, urlGas);
      serializeJsonPretty(nivelesdeluz,Serial);
  webInterface->webPOST(nivelesdeluz, urlGas);
  
  
}
void loop()
{
#ifndef ESP32_RTOS
  ArduinoOTA.handle();
#endif
  sumatoria++;

  horaActual.clear();
  post_ejemplo.clear();
  reloj->getTime();

  horaActual["hora"] = reloj->timeStringBuff;
  horaActual["Sumatoria"] = sumatoria;

  post_ejemplo["Fecha"] = "2020-05-10";
  post_ejemplo["Hora"] = reloj->timeStringBuff;
  post_ejemplo["Temperatura"] = random(30);
  

   // webInterface->webPOST(post_ejemplo, urlPost);
  //  webInterface->webGET(geturl);

  //deserializeJson(get_ejemplo, webInterface->POSTresponse);
  //serializeJsonPretty(get_ejemplo, Serial);
  fugadegas();
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  
  // serializeJson(horaActual,Serial);
}

