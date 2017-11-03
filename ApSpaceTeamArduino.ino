#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

const CE_PIN = 0;//TODO
const CSN_PIN = 0;//TODO

const BTN_1_PIN = 6;
const BTN_2_PIN = 7;
const BTN_3_PIN = 8;

RF24 rf24(CE_PIN, CSN_PIN);

void setup() {
  Serial.begin(57600);
  pinMode(BTN_1_PIN, INPUT);
  rf24.begin();
  rf24.setChannel(13);
  rf24.setPALevel(RF24_PA_MIN);
  //TODO Check the datasheet for restrictions on the intended receiving and transmitting addresses. (For example, 0xc2c2c2c2c2 and 0xe7e7e7e7e7 are good values to use.) In the setup code, set the writing and reading pipes in both sets of code accordingly. Make sure the reading pipe address of the transmitter matches the writing pipe address of the receiver and vice versa.
  rf24.setCRCLength(RF24_CRC_16);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(BTN_1_PIN));
  rf24.startListening();
  //TODO: In the loop, call the available function to check when there are bytes to be read.
  rf24.stopListening();
  delay(10);
}
