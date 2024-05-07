
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>
#include "MAX30102_PulseOximeter.h"


#define OLED_RESET -1
#define WIDTH 128
#define HEIGHT 32
#define REPORTING_PERIOD_MS     1000

Adafruit_MLX90614 temp_sensor = Adafruit_MLX90614();

Adafruit_SSD1306 display(WIDTH, HEIGHT, &Wire, OLED_RESET);
PulseOximeter pox;

uint32_t tsLastReport = 0;
float heart_rate, oxygen_level, old_heart_rate = 0, stress_level;


void onBeatDetected()
{
    Serial.print("Display --> ");
  Serial.println(heart_rate);
}
 
void setup()
{
    Serial.begin(115200);
    Serial.print("Initializing..");
    delay(3000);
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println("display error");
    } else  {
      Serial.println("display on!");
      display.clearDisplay();
    }
    
    if (!temp_sensor.begin()) {
      Serial.println("temp sensor error");
    } else  {
      Serial.println("Temp sensor on!");
    }
    pox.setIRLedCurrent(MAX30102_LED_CURR_7_6MA);
    pox.setOnBeatDetectedCallback(onBeatDetected);
}
 
void loop()
{
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
      display.clearDisplay();
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.print("%");

        heart_rate = pox.getHeartRate();
        old_heart_rate = heart_rate;
        oxygen_level = pox.getSpO2();

        display.setCursor(0, 0);
        display.setTextColor(WHITE);
        display.setTextSize(1);

        display.print("Body Temp: ");
        display.print(temp_sensor.readObjectTempC());
        display.println(" C");

        display.print("Heart Rate: ");
        display.print(heart_rate);
        display.println("BPM");

        display.print("Blood Oxy: ");
        display.print(oxygen_level);
        display.println("%");


        if (heart_rate > old_heart_rate)  {
          stress_level = heart_rate - old_heart_rate;
        } else  {
          stress_level = old_heart_rate - heart_rate;
        }

        display.print("Stress Lvl: ");
        display.print(stress_level);
        display.println("%");

        display.display();
        

    Serial.println();
         
        tsLastReport = millis();
    }
}
