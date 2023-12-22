

#include <WiFiNINA.h>

const char* ssid = "Vodafone-9CEC";
const char* password = "fraNMkfXAb6dR4Jq";

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

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        // Accumulate characters until a newline is received
        if (c == '\n') {
          // Check if the accumulated line contains "POST /toggle"
          if (currentLine.startsWith("POST /toggle")) {
            // Handle toggle request
            toggleLED();
          }

          // Reset the current line
          currentLine = "";

          // Close the connection
          client.stop();
          Serial.println("Client disconnected");
        } else {
          // Accumulate characters in the current line
          currentLine += c;
        }
      }
    }
  }
}


void toggleLED() {
  const int fadeDuration = 1000; // Duration of fade-in and fade-out in milliseconds
  const int fadeSteps = 100;     // Number of steps for fading (adjust as needed)

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



