#include <ESP8266WiFi.h>        
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>

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
  String cookie = logar();
  delay(500);
  String o80221 = "/v2.1/Posicao/Linha?codigoLinha=2085";
  
  http.begin(wifiClient, baseURL+o80221);
  http.addHeader("Cookie", cookie);

  for(int i=0; i<5; i++){
    int httpCode = http.GET();
    String msg = http.getString();
    Serial.println(msg);
    delay(200);
  }
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
