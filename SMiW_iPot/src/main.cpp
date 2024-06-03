// kod Szymona i Mateusza

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

//defining relays pins
#define RELAY1 3
#define RELAY2 4

//defining sensors
#define SEN1 1
#define SEN2 2

//defining buttons pins
#define BUTTON1 5
#define BUTTON2 6
#define BUTTON3 7

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// defining screen's variables
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//humidity % level when started to pump
int allowedHumidityLevel = 50;
boolean screenTrigger = true;
int sensorValue1 = 0;
int humidity1Percent = 0;
int sensorValue2 = 0;
int humidity2Percent = 0;

void IRAM_ATTR ISR_button1() {
  if (screenTrigger) {
    screenTrigger = false;
  }
  else {
    screenTrigger = true;
  }
}

void IRAM_ATTR ISR_button2() {
	if(allowedHumidityLevel > 10) {
		allowedHumidityLevel = allowedHumidityLevel - 10;
	}
}

void IRAM_ATTR ISR_button3() {
	if(allowedHumidityLevel < 100) {
		allowedHumidityLevel = allowedHumidityLevel + 10;
	}
}



void setup() {
  Serial.begin(115200);
  pinMode(RELAY1,OUTPUT);
  digitalWrite(RELAY1, HIGH);
  pinMode(RELAY2,OUTPUT);
  digitalWrite(RELAY2, HIGH);

  pinMode(BUTTON1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON1), ISR_button1, FALLING);
	pinMode(BUTTON2, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(BUTTON2), ISR_button2, FALLING);
	pinMode(BUTTON3, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(BUTTON3), ISR_button3, FALLING);

  //preparing LDC screen
	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
		Serial.println(F("SSD1306 allocation failed"));
		for(;;);
	}
	delay(500);
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
}

void loop() {

  sensorValue1 = analogRead(SEN1);
	humidity1Percent = (3700 - sensorValue1) / 20;
	sensorValue2 = analogRead(SEN2);
	humidity2Percent = (3700 - sensorValue2) / 20;

  if(screenTrigger) {
    display.clearDisplay();
    display.setCursor(0, 10);
    display.print("Sensor 1 %: ");
    display.print(humidity1Percent);
    display.setCursor(0, 20);
    display.print("Sensor 2 %: ");
    display.print(humidity2Percent);
    display.setCursor(0, 30);
    display.print("Border value %: ");
    display.print(allowedHumidityLevel);
    display.display();
  }
  else {
    display.clearDisplay();
    display.display();
  }

  if(humidity1Percent < allowedHumidityLevel){
    digitalWrite(RELAY1,LOW);
    delay(5000);
    digitalWrite(RELAY1,HIGH);
  }

  if(humidity2Percent < allowedHumidityLevel){
    digitalWrite(RELAY2,LOW);
    delay(5000);
    digitalWrite(RELAY2,HIGH);
  }
  
  delay(5000);
}