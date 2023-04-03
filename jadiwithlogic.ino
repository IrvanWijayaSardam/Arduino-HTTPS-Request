#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "SSIDPASS"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
int buzzer = D4;
int button = D3;

const char* host = "cafepagerapp-production.up.railway.app";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char fingerprint[] PROGMEM = "2E F5 0F 8D D6 33 0F 2E C6 CF 84 3B 00 4B 8A 8F 5D 1B 04 61";

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  digitalWrite(buzzer, LOW);
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  static unsigned long lastButtonCheckTime = 0;
  const unsigned long buttonCheckInterval = 100; // Check for button press every 100 ms
  
  if (millis() - lastButtonCheckTime >= buttonCheckInterval) {
    lastButtonCheckTime = millis();
    
    if (digitalRead(button) == LOW) { // If button is pressed...
      Serial.println("Button pressed!");
    }
  }
  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  client.setFingerprint(fingerprint);

  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/api/status/1";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.indexOf("\"data\":true") != -1) {
  digitalWrite(buzzer, HIGH);
  delay(1000); // wait for 2 seconds before turning the pin off
  digitalWrite(buzzer, LOW);// wait for 10 seconds before turning the pin off
  delay(1000);
  digitalWrite(buzzer, HIGH);
  delay(1000); // wait for 2 seconds before turning the pin off
  digitalWrite(buzzer, LOW);// wait for 10 seconds before turning the pin off
  delay(1000);
  digitalWrite(buzzer, HIGH);
  delay(1000); // wait for 2 seconds before turning the pin off
  digitalWrite(buzzer, LOW);// wait for 10 seconds before turning the pin off
  delay(1000);
  digitalWrite(buzzer, HIGH);
  delay(1000); // wait for 2 seconds before turning the pin off
  digitalWrite(buzzer, LOW);// wait for 10 seconds before turning the pin off
  delay(1000);
  digitalWrite(buzzer, HIGH);
  delay(1000); // wait for 2 seconds before turning the pin off
  digitalWrite(buzzer, LOW);// wait for 10 seconds before turning the pin off
  delay(1000);
} else {
  digitalWrite(buzzer, LOW);
  Serial.println("esp8266/Arduino CI has failed");
}
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");

  delay(10000); // Wait 10 seconds before sending the next request
}
