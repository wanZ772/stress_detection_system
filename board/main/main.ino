#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>
#include "MAX30102_PulseOximeter.h"


#define BPM 1000

uint32_t tsLastReport = 0;

#define screen_width 128
#define screen_height 64
#define OLED_RESET 4


PulseOximeter heart_blood;
Adafruit_MLX90614 temp_sensor = Adafruit_MLX90614();
// Adafruit_SSD1306 display(screen_width, screen_height, &Wire, OLED_RESET);
Adafruit_SSD1306 display(OLED_RESET);
float heart_bpm;
int blood_oxy;
void beat() {
  Serial.print("Beat! -> ");
  // Serial.println(heart_blood.getHeartRate());
  // heart_bpm = heart_blood.getHeartRate();
  // heart_blood.update();
}

void setup() {
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  // display.getBuffer();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Please Wait");
  temp_sensor.begin();
  Serial.begin(9600);
  // delay(5000);
  if (!heart_blood.begin())  {
    
    Serial.println("failed");
    for(;;);
  } else  {
    Serial.println("ok");
  }
  display.clearDisplay();
  heart_blood.setIRLedCurrent(MAX30102_LED_CURR_7_6MA);
  heart_blood.setOnBeatDetectedCallback(beat);
  



}

void loop() {
  // double heart_bpm = get_beat();
  // put your main code here, to run repeatedly:


  // display.print("Hello World");
  // display.display();
  // display.clearDisplay();

  heart_blood.update();
  if (millis() - tsLastReport > BPM) {
        // display.clearDisplay();
        Serial.print("Heart rate:");
        Serial.println(heart_blood.getHeartRate());
        // display.write(heart_blood.getHeartRate());
        // display.display();
        // delay(500);
        // display.clearDisplay();
        // display.display();
        // delay(1000);
        // Serial.print("SpO2:");
        // Serial.print(heart_blood.getSpO2());
        // Serial.println("%");
        // Serial.display();
        // delay(2000);
        // display.clearDisplay();
        tsLastReport = millis();
   

    }
  
  // heart_blood.resume();
  // show_display();
}

double get_beat() {
  
  heart_blood.resume();
  if (millis() - tsLastReport > BPM) {
        Serial.print("Heart rate:");
        Serial.print(heart_blood.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(heart_blood.getSpO2());
        Serial.print("%\n");

         
        tsLastReport = millis();
        heart_blood.update();
        return heart_blood.getHeartRate();
    }
}

void show_display() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.print("Body temp: ");
  display.println(temp_sensor.readObjectTempC());

  // display.setCursor(1,1);
  // display.print("Heart Beat: ");
  // display.println(heart_bpm);
  // display.setCursor(2,2);
  display.print("Blood Oxy: ");
  display.print(heart_blood.getSpO2());
  display.println("%");
  display.display();
}
