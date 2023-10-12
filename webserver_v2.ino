#include "defines.h"

//setting static IP
//IPAddress local_ip(192.168.1.1);

//environmental parameters
double temp = 0;
double humidity = 0;
double CO2 = 0;
double soundLv = 0;

#if defined(LED_BUILTIN)
  const int led = LED_BUILTIN;
#else 
  #if(ESP32)
    const int led = 2;
  #else
    const int led = 13;
  #endif
#endif

//Wifi connection status
int status = WL_IDLE_STATUS;

WiFiWebServer server(80);

void handleRoot()
{
  #define BUFFER_SIZE 512

  digitalWrite(led, 1);
  char temp[BUFFER_SIZE];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int day = hr / 24;

  hr = hr % 24;

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
    <img src=\"/test.svg\" />\
    </body>\
    </html>", BOARD_NAME, BOARD_NAME, SHIELD_TYPE, day, hr, min % 60, sec % 60);

  server.send(200, F("text/html"), temp);
  digitalWrite(led, 0);
}

void setup() {
  
  //serial start
  Serial.begin(115200);
  while (!Serial && millis() < 5000);
  
  //WiFi.mode(WIFI_AP);

  //maybe a delay?
  delay(500);

  #if WIFI_USING_ESP_AT  //initialize serial for ESP module and module

    EspSerial.begin(115200);
    WiFi.init(&EspSerial);
  #endif

  pinMode(led, OUTPUT);

  #if (ESP32 || ESP8266)
    WiFi.mode(WIFI_STA);

    Serial.print(F("Connecting to WPA SSID: "));
    Serial.println(ssid);

    if (WiFi.status() != WL_CONNECTED)
    {
      if (strlen(pass) >= 8)
      {
        WiFi.begin(ssid, pass);
      }
      else
      {
        WiFi.begin(ssid);
      }
    }

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
    }

  #else

    //TEMP CODE FOR TESTING PURPOSES
      // check for the presence of the shield
    #if USE_WIFI_NINA

      if (WiFi.status() == WL_NO_MODULE)
    #else
      if (WiFi.status() == WL_NO_SHIELD)
    #endif
      {
        Serial.println(F("WiFi shield not present"));

        // don't continue
        while (true);
      }

    #if USE_WIFI_NINA
      String fv = WiFi.firmwareVersion();
        if (fv < WIFI_FIRMWARE_LATEST_VERSION)
        {
          Serial.println(F("Please upgrade the firmware"));
        }

    #endif

    // attempt to connect to WiFi network
    while ( status != WL_CONNECTED)
    {
      delay(500);
      Serial.print(F("Connecting to WPA SSID: "));
      Serial.println(ssid);
      // Connect to WPA/WPA2 network
      status = WiFi.begin(ssid, pass);
    }

  #endif

  //delay(500);

  //all slash options
  server.on(F("/"), handleRoot);
  server.onNotFound(handlerFunctionNotFound); 
  //server.onFileUpload(handlerAdaptAnimation); //TO DO

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

void handle_OnConnect() //on startup
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

/*void handlerAdaptAnimation()
{
  //Flower animation
  

  //Sky animation



  //clouds/sun animation
}*/



