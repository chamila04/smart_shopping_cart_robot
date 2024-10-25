#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Replace with your network credentials
const char* ssid     = "Chamila's Galaxy A32";
const char* password = "chamila321";

// Server port
WiFiServer server(80);

void scrollText(int row, String message, int delayTime, int lcdColumns) {
  for (int i=0; i < lcdColumns; i++) {
    message = " " + message; 
  } 
  message = message + " "; 
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.setHostname("esp32_board");

  // Connect to Wi-Fi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to Wi-Fi");

  // Start the server
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());

  //LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("WELCOME!");
}

void loop() {
  // Listen for incoming clients
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client connected");

    // Wait until the client sends some data
    while (client.connected() && !client.available()) {
      delay(1);
    }

    // Read and print the message
    String message = client.readStringUntil('\n');
    Serial.println("Message received: " + message);

    //lcd.setCursor(0, 0);
    //lcd.print(message);
    //delay(5000);

    if (message.indexOf("Total") != -1){
      lcd.setCursor(0, 1);
      lcd.print(message);
      delay(3000);
    }
    else{
      scrollText(1, message, 500, 16);
    }

    // Send a response to the client
    client.println("Message received");

    // Close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
}
