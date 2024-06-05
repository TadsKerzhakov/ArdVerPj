#include <Wire.h>
#include <Adafruit_ADXL345_U.h>
#include <MFRC522.h>
#include <SPI.h>
#include <SoftwareSerial.h>

// Configuração do Acelerômetro ADXL345
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// Configuração do RFID RC522
#define RST_PIN         9
#define SS_PIN          10
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Configuração do Bluetooth HC-06
#define BT_SERIAL_RX    2
#define BT_SERIAL_TX    3
SoftwareSerial btSerial(BT_SERIAL_RX, BT_SERIAL_TX);

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);

  // Inicialização do acelerômetro
  if(!accel.begin()) {
    Serial.println("Não foi possível encontrar o sensor ADXL345");
    while(1);
  }

  // Inicialização do RFID
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("Aguardando RFID...");
}

void loop() {
  // Leitura do acelerômetro
  sensors_event_t event;
  accel.getEvent(&event);
  Serial.print("Acelerômetro - X: ");
  Serial.print(event.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(event.acceleration.y);
  Serial.print(", Z: ");
  Serial.println(event.acceleration.z);

  // Leitura do RFID
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("RFID detectado:");
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; ++i) { 
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println(content);
    btSerial.print("RFID detectado: ");
    btSerial.println(content);
    mfrc522.PICC_HaltA();
  }

  delay(1000); // Aguarda 1 segundo
}