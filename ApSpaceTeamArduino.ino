#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#define CE_PIN 9
#define CSN_PIN 10
#define READING_PIPE 0xE7E7E7E7E7
#define WRITING_PIPE 0xC2C2C2C2C2
#define PAYLOAD_SIZE 32

#define BTN_1_PIN 6
#define BTN_2_PIN 7
#define BTN_3_PIN 8

#define LED_1 0x01
#define LED_2 0x02
#define LED_3 0x03

#define CORRECT_BUTTONS 0x04
#define WRONG_BUTTONS 0x05

uint8_t ledSequence[PAYLOAD_SIZE];
uint8_t sequenceIndex = 0;
bool waitForSequence = true;
bool btn1Prev = LOW;
bool btn2Prev = LOW;
bool btn3Prev = LOW;

RF24 rf24(CE_PIN, CSN_PIN);

void setup() {
  Serial.begin(57600);
  pinMode(BTN_1_PIN, INPUT);
  pinMode(BTN_2_PIN, INPUT);
  pinMode(BTN_3_PIN, INPUT);
  rf24.begin();
  rf24.setChannel(13);
  rf24.setPALevel(RF24_PA_MIN);
  rf24.openReadingPipe(1, READING_PIPE);
  rf24.openWritingPipe(WRITING_PIPE);
  rf24.setCRCLength(RF24_CRC_16);
  
  rf24.setPayloadSize(PAYLOAD_SIZE);

  rf24.startListening();
}

void loop() {
  bool btn1State = digitalRead(BTN_1_PIN);
  bool btn2State = digitalRead(BTN_2_PIN);
  bool btn3State = digitalRead(BTN_3_PIN);
  if (waitForSequence) {
    if(rf24.available()) {
      rf24.read(&ledSequence, PAYLOAD_SIZE);
      Serial.println(ledSequence[0]);
      rf24.stopListening();
      waitForSequence = false;
    }
  } else {
    if (btn1State == HIGH && btn1Prev == LOW) {
      checkInput(LED_1);
    } else if (btn2State == HIGH && btn2Prev == LOW) {
      checkInput(LED_2);
    } else if (btn3State == HIGH && btn3Prev == LOW) {
      checkInput(LED_3);
    } 
  }
  btn1Prev = btn1State;
  btn2Prev = btn2State;
  btn3Prev = btn3State;
}

void checkInput(uint8_t LED_NUM) {
  if (LED_NUM == ledSequence[sequenceIndex]) {
    sequenceIndex++;
    if (sequenceIndex == PAYLOAD_SIZE || ledSequence[sequenceIndex] == 0) {
      uint8_t message[1] = {CORRECT_BUTTONS};
      rf24.write(message, 1);
      sequenceIndex = 0;
      waitForSequence = true;
      rf24.startListening();
    }
  } else {
    uint8_t message[1] = {WRONG_BUTTONS};
    rf24.write(message, 1);
    sequenceIndex = 0;
    waitForSequence = true;
    rf24.startListening();
  }
}
