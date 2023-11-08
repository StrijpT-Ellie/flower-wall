#include <WiFi.h>
#include <WebServer.h>

#include <string>

#define BUFFER_SIZE 512

const uint8_t led = 2;

const char* ssid  = "FlowerWall"; //connection point name
const char* pass = "12345671!";  //password


IPAddress local_ip(192, 168, 2, 2);
IPAddress gateway(192, 168, 2, 2);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

void setup() 
{
  //serial start
  Serial.begin(115200);

  while (!Serial && millis() < 5000)
    delay(200);

  Serial.print(F("\nStarting Flower Wall WebServer on "));
  Serial.print("ESP-WHOOP-32");
  Serial.print(F(" with "));
  Serial.println("no_shield");

  //delay for startup
  delay(500);
  
  pinMode(led, OUTPUT);

  WiFi.softAP(ssid, pass);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  //show login credentials on serial board
  Serial.print(F("Starting up Access Point with SSID: "));
  Serial.println(ssid);
  Serial.print(F("with password: "));
  Serial.println(pass);

  delay(500);

  //all slash command options
  server.on("/", handle_OnConnect);
  server.onNotFound(handlerFunctionNotFound);
  //server.on(F("/adapt"), handlerAdaptAnimation); //backlog; blocked by "Implement data gathering"

  server.begin();
  Serial.print(F("Server started at "));
  Serial.println(local_ip);

}

void loop() 
{
  server.handleClient();
}

void handle_OnConnect()  //on startup
{
  Serial.println("Startup Complete");
  server.send(200, "text/html", SendHTML());
}

void handlerFunctionNotFound() 
{
  String message = F("Command not found");

  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");


  Serial.println("Command not found");
  server.send(404, F("Text/plain"), message);
}

String SendHTML(){

  /*int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int day = hr / 24;

  hr = hr % 24;*/

  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
