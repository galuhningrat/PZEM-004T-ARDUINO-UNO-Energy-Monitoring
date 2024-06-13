#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>

// Replace with the appropriate pins for your Arduino board
#define PZEM_RX_PIN 2
#define PZEM_TX_PIN 3

SoftwareSerial pzemSerial(PZEM_RX_PIN, PZEM_TX_PIN);
PZEM004Tv30 pzem(pzemSerial);

void setup() {
  Serial.begin(9600);
  pzemSerial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    switch (command) {
      case 'v':
        sendValue(pzem.voltage());
        break;
      case 'c':
        sendValue(pzem.current());
        break;
      case 'p':
        sendValue(pzem.power());
        break;
      case 'e':
        sendValue(pzem.energy());
        break;
      case 'f':
        sendValue(pzem.frequency());
        break;
      case 'q':
        sendValue(pzem.pf());
        break;
    }
  }
}

void sendValue(float value) {
  Serial.println(value);
}