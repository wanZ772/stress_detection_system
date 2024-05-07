
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>
#include "MAX30102_PulseOximeter.h"


#define BUZZER_PIN 7
#define OLED_RESET -1
#define WIDTH 128
#define HEIGHT 32
#define REPORTING_PERIOD_MS     1000

Adafruit_MLX90614 temp_sensor = Adafruit_MLX90614();

Adafruit_SSD1306 display(WIDTH, HEIGHT, &Wire, OLED_RESET);
PulseOximeter pox;

uint32_t tsLastReport = 0;
int heart_rate = 0, oxygen_level = 0, old_heart_rate = 0, stress_level = 0;


void onBeatDetected()
{
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        heart_rate = int(pox.getHeartRate());
      
      oxygen_level = int(pox.getSpO2());

        


        if (heart_rate > old_heart_rate)  {
          stress_level = heart_rate - old_heart_rate;
        } else  {
          stress_level = old_heart_rate - heart_rate;
        }
        old_heart_rate = heart_rate;

        
        if (stress_level > 30)  {
          digitalWrite(BUZZER_PIN, 1); 
        } else  {
          digitalWrite(BUZZER_PIN, 0);
        }

   
         
        tsLastReport = millis();
  }
}
 
void setup()
{
    pinMode(BUZZER_PIN, OUTPUT);
    Serial.begin(115200);
    Serial.print("Initializing..");

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println("display error");
    } else  {
      Serial.println("display on!");
      display.clearDisplay();
      display.setCursor(0, 0);
        display.setTextColor(WHITE);
        display.setTextSize(2);
        display.print("Developed by Wanz");
        display.display();
    }

    delay(3000);
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

    
    
    if (!temp_sensor.begin()) {
      Serial.println("temp sensor error");
    } else  {
      Serial.println("Temp sensor on!");
    }
    // pox.setIRLedCurrent(MAX30102_LED_CURR_7_6MA);
    pox.setIRLedCurrent(208);
    pox.setOnBeatDetectedCallback(onBeatDetected);

    

        // display.clearDisplay();
}
 
void loop()
{
    pox.update();
        display.setCursor(0, 0);
        display.setTextColor(WHITE);
        display.setTextSize(1);

        display.print("Body Temp: ");
        display.print(temp_sensor.readObjectTempC());
        display.println(" C");

        display.print("Heart Rate: ");
        display.print(heart_rate);
        display.println(" BPM");

        display.print("Blood Oxy: ");
        display.print(oxygen_level);
        display.println("%");
        display.print("Stress Lvl: ");
        display.print(stress_level);
        display.println("%");

        display.display();
        display.clearDisplay();
    // if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
       
    //     Serial.print("Heart rate:");
    //     Serial.print(heart_rate);
    //     Serial.print(" bpm / SpO2:");
    //     Serial.print(oxygen_level);
    //     Serial.print("%");
    //     Serial.print(" / Body Temp: ");
    //     Serial.print(temp_sensor.readObjectTempC());
    //     Serial.print(" C / ");
    //     Serial.print(" Stress Level: ");
    //     Serial.print(stress_level);
    //     Serial.println(" %");
    // }
}
