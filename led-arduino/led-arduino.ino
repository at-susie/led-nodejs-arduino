#include <WiFiNINA.h>
#include "arduino_secrets.h"

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

WiFiServer server(80);
int ledPin = 13;
bool ledState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // Connect to Wi-Fi
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print IP address
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.print("New client connected from ");
    Serial.println(client.remoteIP());

    // Process HTTP request
    String currentLine = "";
    bool headerComplete = false;

    while (client.connected()) {

      // Check if the accumulated line contains "POST /toggle-on"
      if (currentLine.startsWith("POST /toggle-on")) {
        // Handle toggle-on request
        toggleLED();
        Serial.print("Received ON signal");
      }
      if (currentLine.startsWith("POST /toggle-off")) {
        // Handle toggle-off request
        digitalWrite(ledPin, LOW);
        toggleLEDOff();
        Serial.print("Received OFF signal");
      }

      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        if (c == '\n' && currentLine.length() == 0) {
          headerComplete = true;
        } else if (c == '\n') {
          headerComplete = false;

          // // Check if the accumulated line contains "POST /toggle-on"
          // if (currentLine.startsWith("POST /toggle-on")) {
          //   // Handle toggle-on request
          //   toggleLED();
          //   Serial.println("Received ON signal");
          // } else if (currentLine.startsWith("POST /toggle-off")) {
          //   // Handle toggle-off request
          //   digitalWrite(ledPin, LOW);
          //   Serial.println("Received OFF signal");
          // }

          // Reset the current line
          currentLine = "";
        } else if (c != '\r') {
          // Accumulate characters in the current line
          currentLine += c;
        }

        if (headerComplete) {
          // Close the connection
          client.stop();
          Serial.println("Client disconnected");
          break;
        }
      }
    }
  }
}

void toggleLED() {
  Serial.print("Received ON signal");
  const int fadeDuration = 1000;  // Duration of fade-in and fade-out in milliseconds
  const int fadeSteps = 100;      // Number of steps for fading (adjust as needed)

  while (true) {
    // Fade in
    for (int brightness = 0; brightness <= 240; brightness++) {
      analogWrite(ledPin, brightness);
      delay(fadeDuration / fadeSteps);
    }

    // Keep the LED fully on for fadeDuration milliseconds
    delay(fadeDuration);

    // Fade out
    for (int brightness = 240; brightness >= 0; brightness--) {
      analogWrite(ledPin, brightness);
      delay(fadeDuration / fadeSteps);
    }

    // Turn off the LED
    analogWrite(ledPin, 0);

    // Delay between repetitions
    delay(500);
  }
}

void toggleLEDOff() {
  while (true) {
    digitalWrite(ledPin, LOW);
  }

  Serial.print("Received OFF signal");
}