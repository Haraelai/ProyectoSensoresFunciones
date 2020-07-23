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

const char *urlGas = "http://192.168.100.155:8080/ProyectoSensores/public/api/fugasdegas";
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
  nivelesdegas.clear();
  int posicion = random(5);
  nivelesdegas["fechayhora"] = reloj->timeStringBuff;
  nivelesdegas["area"] = zonas [posicion];
  nivelesdegas["niveldegas"] = random(20 + posicion,25 + posicion);
  serializeJsonPretty(nivelesdegas,Serial);
  webInterface->webPOST(nivelesdegas, urlGas);
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

