#include <ESP8266WiFi.h>        
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <math.h>

#define ssid "Vhos" //Nome da rede wi-fi
#define password "yiht0860" //Senha da rede
#define baseURL "http://api.olhovivo.sptrans.com.br"
#define token "dbc75b095b00a61d2011cf398eeb6adbba4cf61ef9cec1c40c90e9e36c6c1cf0"

const char * headerKeys[] = {"Set-Cookie"};
const size_t numberOfHeaders = 1;



HTTPClient http;
WiFiClient wifiClient;

void setup() {
  
  Serial.begin(9600); 
  delay(10);
  
  pinMode(BUILTIN_LED, OUTPUT); 
  digitalWrite(BUILTIN_LED, LOW); 
  
  WiFi.begin(ssid, password);             
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");
  
  while (WiFi.status() != WL_CONNECTED) { 
    delay(100);   
  } 
   
  digitalWrite(BUILTIN_LED, HIGH);
  Serial.println('\n');
  Serial.println("Connection established!");

}

void loop() {

  float xIPT = -23.55601721442918;
  float yIPT = -46.733710521997594;
  String cookie = logar();
  delay(1000);
  String o80221 = "/v2.1/Posicao/Linha?codigoLinha=2085"; //onibus 8022 direcao metro
  
  http.begin(wifiClient, baseURL+o80221);
  http.addHeader("Cookie", cookie);
  
  int httpCode = http.GET();
  String msg = http.getString();

  StaticJsonDocument<1000> doc;
  DeserializationError error = deserializeJson(doc, msg);

   if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());

   }

   float *dTerminal = new float[1];
   dTerminal[0] = sqrt((-23.551950231465387+23.57195464211519)*(-23.551950231465387+23.57195464211519)+(-46.73183795190236+46.70897838755912)*(-46.73183795190236+46.70897838755912));
   
   float *px1 = new float[1];
   float *py1 = new float[1];
   px1[0] = doc["vs"][0]["px"];
   py1[0] = doc["vs"][0]["py"];

   float d = sqrt((px1[0]-xIPT)*(px1[0]-xIPT)+(py1[0]-yIPT)*(py1[0]-yIPT));
   Serial.println(px1[0], 10);
   Serial.println(py1[0], 10);



   delete[] px1;
   delete[] py1;
   
   float t = d * 20/(dTerminal[0]);
    
   Serial.println(dTerminal[0], 10);
   delete[] dTerminal;
    
   Serial.println(d, 10);
   Serial.print(t, 3);Serial.println("minutos");
    
   http.end();
   delay(1000);
   
}

String logar(){
  String cookie;
  String URLauth = "/v2.1/Login/Autenticar?token=";
  HTTPClient http;
  
  http.begin(wifiClient,baseURL + URLauth + token);
  http.collectHeaders(headerKeys, numberOfHeaders);
  int httpCode = http.GET();
  http.POST("");
  
  for(int i = 0; i< http.headers(); i++){
    cookie = (http.header(i));
  }

  http.end();
  
  return cookie;
  
 }
