#include "defines.h"
#include "EvnironmentData.ino"

#define BUFFER_SIZE 512

const int led = 2;

/*#if defined(LED_BUILTIN)
  const int led = LED_BUILTIN;
#else
  #if (ESP32)
    const int led = 2;
  #else
    const int led = 13;
  #endif
#endif*/

//AP station status
int status = WL_IDLE_STATUS;

WiFiWebServer server(80);

void handleRoot() 
{
  digitalWrite(led, 1);

  char temp[BUFFER_SIZE];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int day = hr / 24;

  hr = hr % 24;

  //startup web page upon accessing the arduino's IP address; contains basic info on the setup components
  snprintf(temp, BUFFER_SIZE - 1,
    "<html>\
    <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>%s</title>\
    <style>\
    body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
    </head>\
    <body>\
    <h1>Hello from %s</h1>\
    <h3>running WiFiWebServer</h3>\
    <h3>on %s</h3>\
    <p>Uptime: %d d %02d:%02d:%02d</p>\
    </body>\
    </html>", BOARD_NAME, BOARD_NAME, SHIELD_TYPE, day, hr, min % 60, sec % 60);

  server.send(200, F("text/html"), temp);
  digitalWrite(led, 0);
}

void setup() 
{
  //serial start
  Serial.begin(115200);
  while (!Serial && millis() < 5000)
    delay(200);

  Serial.print(F("\nStarting Flower Wall WebServer on "));
  Serial.print(BOARD_NAME);
  Serial.print(F(" with "));
  Serial.println(SHIELD_TYPE);
  Serial.println(WIFI_WEBSERVER_VERSION);

  //delay for startup
  delay(500);

  #if WIFI_USING_ESP_AT  //initialize serial for ESP module
    EspSerial.begin(115200);
    WiFi.init(&EspSerial);

  #endif
    pinMode(led, OUTPUT);

  #if (ESP32 || ESP8266)
    WiFi.mode(WIFI_AP);

    //show login credentials on serial board
    Serial.print(F("Starting up Access Point with SSID: "));
    Serial.println(ssid);
    Serial.print(F("with password: "));
    Serial.println(pass);

  #else
    uint8_t ap_channel = 2;

    status = WiFi.beginAP(ssid, pass, ap_channel);

    if(status != WL_AP_LISTENING)
    {
      Serial.println(F("AP creation failed"));
      while(true);
    }
  #endif
  //delay(500);

  //all slash command options
  server.on(F("/"), handleRoot);
  server.onNotFound(handlerFunctionNotFound);
  //server.on(F("/adapt"), handlerAdaptAnimation); //backlog; blocked by "Implement data gathering"

  server.begin();
  Serial.print(F("Server started at "));
  Serial.println(WiFi.localIP());
  getWiFiStatus();
}

void loop() 
{
  server.handleClient();
}

void getWiFiStatus() 
{
  Serial.println(F("SSID: "));
  Serial.println(WiFi.SSID());

  IPAddress IP = WiFi.localIP();

  Serial.println(F("IP address: "));
  Serial.println(IP);
}

void handle_OnConnect()  //on startup
{
  //start basic animation
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


