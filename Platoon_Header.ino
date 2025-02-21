#include <WiFi.h>
#include <WebServer.h>
#include <RF24.h>

// NRF24L01 Pins
#define CE_PIN   5
#define CSN_PIN  4
RF24 radio(CE_PIN, CSN_PIN);

// WiFi credentials 
const char* ssid = "Platoon_Header";
const char* password = "123456789";

// WebServer 
WebServer server(80);

// Motor control pins
#define IN1 13
#define IN2 12
#define EN1 14
#define IN3 27
#define IN4 26
#define EN2 25

byte addresses[][6] = {"1Node", "2Node"}; // Radio addresses

void setup() {
  Serial.begin(115200);


  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN2, OUTPUT);

  
  WiFi.softAP(ssid, password);
  Serial.println("Wi-Fi Access Point Started");

  
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  radio.startListening();

 
  server.on("/", handleRoot);
  server.on("/control", HTTP_POST, handleControl);
  server.begin();
  Serial.println("Web Server Started");
}

void loop() {
  server.handleClient();
  handleRadioCommunication();
}

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
  <style>
    .header {
      font-size: 24px;
      font-weight: bold;
      text-align: center;
      margin-top: 10px;
      margin-bottom: 20px;
    }
    .arrows { 
      font-size: 40px; 
      color: red; 
    }
    td.button { 
      background-color: black; 
      border-radius: 25%; 
      box-shadow: 5px 5px #888888; 
    }
    td.button:active { 
      transform: translate(5px, 5px); 
      box-shadow: none; 
    }
    .noselect { 
      user-select: none; 
    }
  </style>
</head>
<body class="noselect" align="center" style="background-color: white">
  <!-- Header Section -->
  <div class="header">PLATOON_SYSTEM-HEADER</div>

  <!-- Control Buttons -->
  <table style="width:400px; margin:auto; table-layout:fixed" cellspacing="10">
    <tr>
      <td></td>
      <td class="button" ontouchstart="sendInput('MoveCar','1')" ontouchend="sendInput('MoveCar','0')">
        <span class="arrows">&#8679;</span>
      </td>
      <td></td>
    </tr>
    <tr>
      <td class="button" ontouchstart="sendInput('MoveCar','3')" ontouchend="sendInput('MoveCar','0')">
        <span class="arrows">&#8678;</span>
      </td>
      <td class="button"></td>
      <td class="button" ontouchstart="sendInput('MoveCar','4')" ontouchend="sendInput('MoveCar','0')">
        <span class="arrows">&#8680;</span>
      </td>
    </tr>
    <tr>
      <td></td>
      <td class="button" ontouchstart="sendInput('MoveCar','2')" ontouchend="sendInput('MoveCar','0')">
        <span class="arrows">&#8681;</span>
      </td>
      <td></td>
    </tr>
  </table>

  <!-- JavaScript for sending commands -->
  <script>
    function sendInput(key, value) {
      fetch('/control', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: ${key}=${value}
      });
    }
  </script>
</body>
</html>

)rawliteral";

  server.send(200, "text/html", html);
}


void handleControl() {
  if (server.hasArg("MoveCar")) {
    String direction = server.arg("MoveCar");
    if (direction == "1") { moveForward(); }
    else if (direction == "2") { moveBackward(); }
    else if (direction == "3") { turnLeft(); }
    else if (direction == "4") { turnRight(); }
    else { stopCar(); }
  }
  server.send(200, "text/plain", "OK");
}

void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(EN1, 255); analogWrite(EN2, 255);
  sendRadioMessage("FORWARD");
  Serial.println("Moving Forward");
}

void moveBackward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(EN1, 255); analogWrite(EN2, 255);
  sendRadioMessage("BACKWARD");
  Serial.println("Moving Backward");
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(EN1, 255); analogWrite(EN2, 255);
  sendRadioMessage("LEFT");
  Serial.println("Turning Left");
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(EN1, 255); analogWrite(EN2, 255);
  sendRadioMessage("RIGHT");
  Serial.println("Turning Right");
}

void stopCar() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(EN1, 0); analogWrite(EN2, 0);
  sendRadioMessage("STOP");
  Serial.println("Car Stopped");
}


void sendRadioMessage(String message) {
  radio.stopListening();  
  radio.write(message.c_str(), message.length() + 1);
  radio.startListening(); 
}

void handleRadioCommunication() {
  if (radio.available()) {
    char incomingMessage[32] = {0};
    radio.read(&incomingMessage, sizeof(incomingMessage));
    Serial.println("Received: " + String(incomingMessage));
  }
}
