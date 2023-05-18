#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
byte nuidPICC[4];
int buzzer = 6;
int greenLed = 5;
int redLed = 7;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  pinMode(buzzer, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  digitalWrite(buzzer, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, LOW);
}

void loop(){
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String cardID = "";
    for (byte i = 0; i < 4; i++) {
      cardID += (rfid.uid.uidByte[i] < 0x10 ? "0" : "") + String(rfid.uid.uidByte[i], HEX);
    }
    Serial.println(cardID); // Print card ID on serial monitor

    // Wait for validation response from Python
    while (Serial.available() <= 0);
    delay(500);
    char response = Serial.read();

    if (response == 'A') {
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
      tone(buzzer, 2000, 500);
      delay(1000);
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, LOW);
    } else if (response == 'D') {
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
      tone(buzzer, 500, 100);
      delay(1000);
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, LOW);
    }
  }

  /*
   * Halt PICC
   * Stop encryption on PCD
   */
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
