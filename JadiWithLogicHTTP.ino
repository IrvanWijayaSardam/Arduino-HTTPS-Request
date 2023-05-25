#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "Wastian";
const char* password = "123456789";

//Your Domain name with URL path or IP address with path
String serverName = "http://157.245.55.214:8080/api";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 60000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 3000;
int buzzer = D4;
int button = D3;
bool isBuzzing = false;
unsigned long stopTime = 0;

void setup() {
  Serial.begin(115200); 
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  // Send an HTTP POST request depending on timerDelay
  if (digitalRead(button) == LOW) {
    // Button is pressed
    Serial.println("Button pressed...");
    stopTime = millis() + 50000; // Set stop time 1 minute from now
    isBuzzing = false; // Stop buzzing
  }

  // Check if it's time to start buzzing again
  if (!isBuzzing && millis() >= stopTime) {
    isBuzzing = true; // Start buzzing again
    stopTime = 0; // Reset stop time
  }

  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      String serverPath = serverName + "/pager/2";
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
  
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
        
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);

      if (payload.indexOf("\"xstatus\":true") != -1) {
        Serial.println("xstatus is true");
      // Buzz the buzzer
      if (isBuzzing) {
          // Buzz the buzzer
          for (int i = 0; i < 25; i++) {
            Serial.println("Should be buzzing");
            digitalWrite(buzzer, HIGH);
            delay(200); // Wait for 200 milliseconds before turning the pin off
            digitalWrite(buzzer, LOW);
            delay(200); // Wait for 200 milliseconds before beeping again
          }
        } else {
          digitalWrite(buzzer, LOW);
        }
        delay(5000);
        Serial.println("Delay 10 sechond");
      } else {
        digitalWrite(buzzer, LOW);
      }
        
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
