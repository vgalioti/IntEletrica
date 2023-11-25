#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>
#include <math.h>

const char* ssid = "Nome da rede"; //Rede wifi
const char* password = "Senha da rede"; //Senha wifi

const char * headerKeys[] = {"Set-Cookie"};
const size_t numberOfHeaders = 1;

String authCode;
String msg;

HTTPClient https;
std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

void conWifi();
void startHttps();
String logar();
int getPosition();

void setup() {
  Serial.begin(115200);
  delay(100);
  conWifi();
  startHttps();
  authCode = logar();
}

void loop() {
  int oldTempo = 20;
  if(getPosition() == 200){
    int tempo = calcularTempo();
    if(tempo != -1 && tempo<30){
      oldTempo = tempo;
    }
  }

  Serial.println(oldTempo);
  
  delay(1000);
}

void conWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting...");
    delay(1000);
  }  
}

void startHttps(){

   if ((WiFi.status() == WL_CONNECTED)) {
    client->setInsecure();
    if (https.begin(*client, "https://www.howsmyssl.com/a/check")) {  // HTTPS
      // start connection and send HTTP header
      int httpCode = https.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
             Serial.println("[HTTPS] stablished");     
  
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        https.end();
      }
 
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
 }
}


String logar(){
  String cookie;
  https.collectHeaders(headerKeys, numberOfHeaders);
  https.begin(*client, "https://api.olhovivo.sptrans.com.br/v2.1/Login/Autenticar?token=dbc75b095b00a61d2011cf398eeb6adbba4cf61ef9cec1c40c90e9e36c6c1cf0");
  
  if(https.POST("")){
    Serial.println("Login feito");
    for (int i = 0; i < https.headers(); i++) {
       cookie = (https.header(i));
    }
     
  https.end();             
  }
  else{Serial.println("Falha no login");}

  return cookie;
}

int getPosition(){
  https.begin(*client, "http://api.olhovivo.sptrans.com.br/v2.1/Posicao/Linha?codigoLinha=34853"); //Onibus 8022 na direcao metro
  https.addHeader("Cookie", authCode);
  delay(500);
  msg = https.getString();
  return https.GET();
  
}


int calcularTempo(){
  float x,y, d, xEstacao = -23.572095801613287, yEstacao = -46.70809516536287, xTerm =-23.552449171677665, yTerm = -46.73167825334665 ;
  int temp;
  StaticJsonDocument<1000> doc;
  DeserializationError error = deserializeJson(doc, msg);
 
  if (error) return -1;
  x = doc["vs"][0]["py"]; //API da SPTRANS inverte latitude e longitude
  y = doc["vs"][0]["px"];
  d = sqrt(pow((x-xEstacao),2)+pow((y-yEstacao),2));

  temp = 20*d/(sqrt(pow((xTerm-xEstacao),2)+pow((yTerm-yEstacao),2)));
  
  return temp;
  
}
