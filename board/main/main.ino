
#include <Wire.h>
// #include <SPI.h>
// #include <Adafruit_SSD1306.h>
// #include <Adafruit_GFX.h>
#include "U8glib.h"
#include <Adafruit_MLX90614.h>
#include "MAX30100_PulseOximeter.h"


#define BUZZER_PIN 7
#define OLED_RESET -1
#define WIDTH 128
#define HEIGHT 64
#define REPORTING_PERIOD_MS 1000

Adafruit_MLX90614 temp_sensor = Adafruit_MLX90614();
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);
// Adafruit_SSD1306 display(WIDTH, HEIGHT, &Wire, OLED_RESET);

PulseOximeter pox;

uint32_t tsLastReport = 0;
int heart_rate = 0, oxygen_level = 0, old_heart_rate = 0, stress_level = 0, to_stop=0, ave = 0;
int ave_hr[20];
int ave_bo[20];



// 'image_2024-06-09_195031043', 128x64px
// 'image_2024-06-09_195031043', 128x64px
const unsigned char epd_bitmap_image_2024_06_09_195031043 [] PROGMEM = {
	0x21, 0xfe, 0xff, 0x80, 0xbf, 0x8b, 0xdf, 0xef, 0xff, 0x7f, 0xff, 0xff, 0xa3, 0xff, 0xff, 0xff, 
	0x02, 0xd6, 0x16, 0x00, 0xfe, 0x17, 0xee, 0xdf, 0xff, 0x7f, 0xff, 0xff, 0x80, 0xff, 0x5b, 0x7f, 
	0x03, 0xfa, 0x03, 0x00, 0xf4, 0x1e, 0x9c, 0xbf, 0xfd, 0xdb, 0xdf, 0x0b, 0x00, 0xfc, 0xfe, 0x3f, 
	0x07, 0x58, 0x00, 0x00, 0x40, 0x7c, 0x90, 0xf7, 0x7f, 0xff, 0xf7, 0x01, 0x00, 0x40, 0xff, 0x0d, 
	0x0b, 0x20, 0x00, 0x00, 0x00, 0xb8, 0x80, 0xff, 0xeb, 0xff, 0x7d, 0x00, 0x00, 0x00, 0xb8, 0x87, 
	0x0e, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0xff, 0xd0, 0x4a, 0x5f, 0x00, 0x00, 0x00, 0xe0, 0xc7, 
	0x0e, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0x5f, 0x80, 0x0f, 0x05, 0x00, 0x00, 0x00, 0xc0, 0xa5, 
	0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x08, 0x82, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x80, 0xe3, 
	0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe3, 
	0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x3b, 0xdf, 0x01, 0x00, 0x00, 0x00, 0x00, 0xd2, 
	0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 0x6f, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 
	0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb4, 0x3b, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 
	0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 
	0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 
	0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x6f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x30, 
	0x0e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x86, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x38, 0x7f, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x0d, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbc, 0x57, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 
	0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb8, 0x3f, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 
	0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x7b, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 
	0x8e, 0x04, 0x00, 0x00, 0x00, 0x00, 0x10, 0xdc, 0x1f, 0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x9a, 
	0x86, 0x02, 0x00, 0x00, 0x00, 0x00, 0x30, 0xf8, 0x3f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9f, 
	0x8e, 0x08, 0x10, 0x00, 0x00, 0x00, 0x18, 0x5c, 0x6b, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8d, 
	0x8e, 0x2b, 0x00, 0x00, 0x00, 0x00, 0x14, 0xd8, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0xdf, 
	0x1c, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x0e, 0xbc, 0x6f, 0x01, 0x20, 0x00, 0x00, 0x00, 0x80, 0x87, 
	0x1c, 0xc5, 0xda, 0x00, 0x00, 0x00, 0x05, 0xb8, 0x5d, 0x01, 0xe0, 0x00, 0x40, 0xa0, 0x80, 0xcf, 
	0x1c, 0x9e, 0x37, 0x18, 0x40, 0x80, 0x07, 0x3e, 0x27, 0x28, 0x41, 0x01, 0x00, 0xe0, 0x80, 0x07, 
	0x34, 0x1c, 0x1a, 0x58, 0x00, 0xc0, 0x01, 0x6f, 0x6f, 0x3d, 0xc7, 0x05, 0x00, 0xb1, 0xc1, 0x0d, 
	0x38, 0x74, 0x00, 0xc0, 0x03, 0xf0, 0x81, 0xd1, 0x23, 0xfa, 0x03, 0xfb, 0xe9, 0x62, 0x80, 0xc3, 
	0x78, 0xfc, 0x01, 0x90, 0x01, 0x78, 0x00, 0xc1, 0x63, 0xac, 0x06, 0x00, 0x18, 0x05, 0xe0, 0xc7, 
	0x78, 0xf0, 0x0f, 0x00, 0x01, 0x3e, 0x80, 0xc1, 0x45, 0xfd, 0x03, 0x00, 0x00, 0x00, 0xe8, 0xe1, 
	0xd0, 0xc0, 0x1f, 0x02, 0xc0, 0x1f, 0x00, 0x01, 0x41, 0xc3, 0xd7, 0xc0, 0x07, 0x00, 0x7c, 0xc3, 
	0xf0, 0x80, 0x36, 0x5f, 0xa0, 0x17, 0x80, 0x01, 0xc0, 0x01, 0x7d, 0xeb, 0x1f, 0x48, 0xdf, 0xe0, 
	0x60, 0x01, 0x88, 0x1f, 0xe4, 0x06, 0x00, 0x03, 0x80, 0x19, 0xdf, 0x7b, 0x1b, 0xfe, 0x7b, 0xa8, 
	0xc0, 0x01, 0x00, 0x04, 0xe3, 0xab, 0x00, 0x07, 0xc0, 0x79, 0xfb, 0xfe, 0x7e, 0xfe, 0x5e, 0xe8, 
	0xc0, 0x57, 0x3f, 0xc0, 0xa6, 0x7b, 0x80, 0x03, 0x80, 0xdc, 0xbe, 0xaf, 0x6f, 0xda, 0x17, 0x46, 
	0x00, 0xff, 0xeb, 0xe0, 0x41, 0x54, 0x80, 0x01, 0x00, 0x78, 0xe8, 0xff, 0xfd, 0xfd, 0x06, 0xad, 
	0x00, 0x7c, 0xff, 0x7b, 0x03, 0x7c, 0xc0, 0x01, 0x00, 0xec, 0x50, 0x7f, 0xbb, 0xa2, 0x40, 0xa5, 
	0x00, 0xe8, 0xff, 0xff, 0x00, 0x37, 0xc0, 0x00, 0x00, 0x78, 0x00, 0xe8, 0x0f, 0x00, 0xc0, 0x4a, 
	0x00, 0x00, 0x15, 0xde, 0xc0, 0x1f, 0x60, 0x00, 0x01, 0xdc, 0x10, 0x40, 0x05, 0x00, 0xb0, 0xc2, 
	0x80, 0x00, 0x00, 0x10, 0xe0, 0x05, 0x20, 0x80, 0x00, 0x78, 0x7c, 0x00, 0x03, 0x00, 0xf6, 0x23, 
	0x11, 0x00, 0x00, 0x00, 0x7c, 0x01, 0x30, 0x80, 0x00, 0x30, 0x1d, 0x00, 0x00, 0xe0, 0x5d, 0x51, 
	0x05, 0x10, 0x00, 0x00, 0x5f, 0x00, 0x30, 0x40, 0x00, 0x78, 0x3f, 0x02, 0x00, 0x8d, 0xdf, 0xb8, 
	0x40, 0x14, 0x00, 0xc0, 0x05, 0x00, 0x18, 0x00, 0x00, 0x50, 0x0f, 0x1d, 0x00, 0xa9, 0xec, 0x3d, 
	0x50, 0x25, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xb8, 0x9b, 0x0e, 0x00, 0x76, 0x79, 0x1c, 
	0x45, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xb8, 0x9f, 0x1e, 0x00, 0xdd, 0xb5, 0x37, 
	0xa0, 0x1d, 0x24, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x98, 0x8e, 0x0d, 0x00, 0x7c, 0x73, 0xcf, 
	0xd0, 0x16, 0x20, 0x01, 0x00, 0x00, 0xf0, 0x00, 0x00, 0xb8, 0x6f, 0x07, 0x00, 0xf0, 0xd7, 0xfd, 
	0xac, 0x3f, 0x08, 0x44, 0x00, 0x00, 0x18, 0x00, 0x00, 0xb0, 0xd7, 0x07, 0x40, 0xa8, 0xf7, 0xbf, 
	0xf4, 0x0f, 0x40, 0x09, 0x00, 0x00, 0x1c, 0x00, 0x02, 0xf0, 0xef, 0x03, 0x00, 0xc0, 0xad, 0xff, 
	0x7e, 0x00, 0x12, 0x90, 0x80, 0x00, 0x0c, 0x00, 0x00, 0xe0, 0xed, 0x03, 0x00, 0x80, 0xef, 0x77, 
	0x1e, 0x80, 0x04, 0x06, 0x04, 0x00, 0x1c, 0x00, 0x01, 0xe0, 0x7f, 0x03, 0x00, 0x00, 0xeb, 0xfd, 
	0x06, 0x90, 0x2a, 0x40, 0x00, 0x00, 0x0c, 0x00, 0x02, 0x80, 0xfb, 0x07, 0x09, 0x22, 0xc6, 0xff, 
	0x06, 0xd0, 0x5d, 0x21, 0x10, 0x00, 0x0e, 0x00, 0x01, 0x80, 0xdf, 0x03, 0x92, 0x80, 0xcc, 0xdf, 
	0x03, 0x60, 0xff, 0x00, 0x01, 0x00, 0x06, 0x00, 0x02, 0x80, 0xff, 0x47, 0x1e, 0x00, 0x89, 0xfb, 
	0x02, 0xd8, 0xb7, 0x97, 0x02, 0x00, 0x02, 0x00, 0x03, 0x10, 0xe7, 0xa3, 0xa4, 0x01, 0x90, 0xfe, 
	0x04, 0x74, 0xff, 0x06, 0x04, 0x00, 0x03, 0x00, 0x01, 0x04, 0xf7, 0xf2, 0x05, 0x91, 0x30, 0xbf, 
	0x40, 0xa8, 0xfd, 0x2d, 0x28, 0x01, 0x02, 0x00, 0x03, 0x08, 0xe7, 0xbb, 0x01, 0x02, 0xa0, 0xf8, 
	0x00, 0x60, 0x6f, 0xab, 0x12, 0x08, 0x01, 0x00, 0x06, 0x10, 0xf7, 0xf5, 0x00, 0x08, 0x70, 0xd5, 
	0x00, 0x54, 0xff, 0x4b, 0x21, 0x00, 0x02, 0x02, 0x05, 0x08, 0xe7, 0x7a, 0x80, 0x00, 0xc0, 0x02, 
	0x80, 0x94, 0x6a, 0xb5, 0x8a, 0x00, 0x01, 0x80, 0x0e, 0x84, 0xe7, 0xbe, 0x08, 0x01, 0x80, 0x01, 
	0x00, 0x52, 0x7d, 0x4b, 0x04, 0x00, 0x82, 0x44, 0x03, 0x08, 0xf7, 0x3d, 0x40, 0x10, 0x00, 0x0b, 
	0x80, 0xf6, 0xad, 0x14, 0x41, 0x00, 0x02, 0xc1, 0x05, 0x82, 0xf3, 0x5e, 0x00, 0x01, 0x10, 0x05, 
	0x88, 0xbe, 0x5f, 0xbf, 0x04, 0x00, 0x04, 0xe2, 0x07, 0xc0, 0xf9, 0x1f, 0x64, 0x84, 0x40, 0x06
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 1040)
const int epd_bitmap_allArray_LEN = 1;
const unsigned char* epd_bitmap_allArray[1] = {
	epd_bitmap_image_2024_06_09_195031043
};





void onBeatDetected() {
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        heart_rate = int(pox.getHeartRate());
        
        int current_oxygen = pox.getSpO2();
        if (current_oxygen != 0)  {
          if (current_oxygen > 100) {
            oxygen_level = 100;
          } else  {
            if ((current_oxygen < oxygen_level) && ((oxygen_level - current_oxygen) < 10))  {
              oxygen_level = current_oxygen;
          } 
        }


        if (heart_rate > old_heart_rate)  {
          stress_level = heart_rate - old_heart_rate;
        } else  {
          stress_level = old_heart_rate - heart_rate;
        }
        old_heart_rate = heart_rate;
        tsLastReport = millis();
        Serial.print(oxygen_level);
        Serial.print(" / ");
        Serial.println(tsLastReport);

  }
} 
  if (to_stop < 20) {
    Serial.println(to_stop);
    ave_hr[to_stop] = heart_rate;
    to_stop++; 
    
  } else  {
    pox.shutdown();
    for (int i = 0; i < 20; i++)  {
      ave += (ave_hr[i]);
      
    }
    Serial.print("Average: ");
    Serial.println(ave / 20);
    heart_rate = ave / 20;
  }
}
void clear_oled(){
u8g.firstPage();
do {
  u8g.drawXBMP( 0, 0, 128, 64, epd_bitmap_image_2024_06_09_195031043);
} while( u8g.nextPage() );
delay(5000);
}
void setup()
{
    pinMode(BUZZER_PIN, OUTPUT);
    Serial.begin(115200);
    clear_oled();
    
  // u8g.setFont(u8g_font_osb21);
  
    // delay(3000);
    if (!pox.begin()) {
        Serial.println("[ - ] MAX30100 OFFLINE");
    } else {
        Serial.println("[ + ] MAX30100 ONLINE");
       
    }

    
    
    if (!temp_sensor.begin()) {
     Serial.println("[ - ] MLX90614 OFFLINE");
    } else  {
      Serial.println("[ + ] MLX90614 ONLINE");
        
    }
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    // pox.setIRLedCurrent(200);
    pox.setOnBeatDetectedCallback(onBeatDetected);


    
    u8g.setCursorPos(0, 0);
    u8g.setFont(u8g_font_6x12);
    

    

        // display.clearDisplay();
}
 
void loop()
{
  String test = "test";
    
   
   Serial.println(heart_rate);
u8g.firstPage();  
  do {
    pox.update();
    u8g.setPrintPos(0, 20);
    u8g.print("Heart Rate: ");
    u8g.println(heart_rate);
    u8g.setPrintPos(0, 30);
    u8g.print("Oxygen Level: ");
    u8g.println(pox.getSpO2());
    u8g.setPrintPos(0, 40);
    u8g.print("Body Temp: ");
    u8g.println(temp_sensor.readObjectTempC());
  } while( u8g.nextPage() );
    // delay(2000);
  // clear_oled();
        
        
        // display.setCursor(0, 0);
        // display.setTextColor(WHITE);
        // display.setTextSize(1);

        // Serial.print("Body Temp: ");
        // Serial.print(temp_sensor.readObjectTempC());
        // // Serial.println(temp_sensor.readObjectTempC());
        // Serial.println(" C");

        // Serial.print("Heart Rate: ");            
        // Serial.print(heart_rate);
        // Serial.println(" BPM");

        // Serial.print("Blood Oxy: ");
        // Serial.print(oxygen_level);
        // Serial.println("%");
       
        // display.print("Stress Lvl: ");
        // display.print(stress_level);
        // display.println("%");

        // display.display();
        // display.clearDisplay();
}
