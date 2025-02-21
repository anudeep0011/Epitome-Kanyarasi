#include <RF24.h>

// NRF24L01 Pins
#define CE_PIN   5
#define CSN_PIN  4
RF24 radio(CE_PIN, CSN_PIN);

byte addresses[][6] = {"1Node", "2Node"};

#define IN1 13
#define IN2 12
#define EN1 14

#define IN3 27
#define IN4 26
#define EN2 25



void setup() {
  Serial.begin(115200);

  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN2, OUTPUT);

 
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(addresses[1]);  
  radio.openReadingPipe(1, addresses[0]); 
  
  radio.startListening();  
}

void loop() {
  if (radio.available()) {
    char receivedData[32] = "";
    radio.read(&receivedData, sizeof(receivedData));
    Serial.print("Received: ");
    Serial.println(receivedData);

    if (strcmp(receivedData, "FORWARD") == 0) {
      // Move car forward
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(EN1, 255);  
      analogWrite(EN2, 255);  
    } else if (strcmp(receivedData, "BACKWARD") == 0) {
      // Move car backward
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(EN1, 255); 
      analogWrite(EN2, 255);  
    } else if (strcmp(receivedData, "LEFT") == 0) {
      // Turn car left
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(EN1, 255);  
      analogWrite(EN2, 255);  
    } else if (strcmp(receivedData, "RIGHT") == 0) {
      // Turn car right
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(EN1, 255);  
      analogWrite(EN2, 255);  
    } else if (strcmp(receivedData, "STOP") == 0) {
      // Stop car
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      analogWrite(EN1, 0);  
      analogWrite(EN2, 0); 
      
       
    }
  }
}