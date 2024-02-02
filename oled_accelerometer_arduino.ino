

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_tockn.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

MPU6050 mpu6050(Wire);

float bubbleX = SCREEN_WIDTH / 2.0;
float bubbleY = SCREEN_HEIGHT / 2.0;
float bubbleRadius = 8.0;  // Increased bubble size
float sensitivityFactor = 2.0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();

  mpu6050.begin();
}

void loop() {
  mpu6050.update();

  float ay = mpu6050.getAccX() * 55;
  float ax = mpu6050.getAccY() * 55;

  bubbleX += ax;
  bubbleY += ay;

  bubbleX = constrain(bubbleX, bubbleRadius, SCREEN_WIDTH - 1 - bubbleRadius);
  bubbleY = constrain(bubbleY, bubbleRadius, SCREEN_HEIGHT - 1 - bubbleRadius);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Fill the circle with active pixels
  for (int y = -bubbleRadius; y <= bubbleRadius; y++) {
    for (int x = -bubbleRadius; x <= bubbleRadius; x++) {
      if (x * x + y * y <= bubbleRadius * bubbleRadius) {
        display.drawPixel(bubbleX + x, bubbleY + y, SSD1306_WHITE);
      }
    }
  }
  
  display.display();
  
  delay(1);
}
