#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "qrcode.h" 

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }

  display.clearDisplay();

  const char* url = "https://www.google.com";

  esp_qrcode_config_t cfg = ESP_QRCODE_CONFIG_DEFAULT();
  cfg.display_func = NULL; 
  cfg.max_qrcode_version = 10;
  cfg.qrcode_ecc_level = ESP_QRCODE_ECC_LOW;

  // FORCE CAST: We treat the return of the generate function as the handle.
  // This matches versions where the handle is the return value.
  esp_qrcode_handle_t qr_handle = (esp_qrcode_handle_t)esp_qrcode_generate(&cfg, url);

  if (qr_handle != NULL) {
    int qrSize = esp_qrcode_get_size(qr_handle);
    
    // Scale 2 centered on 2.44 cm screen
    int scale = 2; 
    int xOffset = (SCREEN_WIDTH - (qrSize * scale)) / 2;
    int yOffset = (SCREEN_HEIGHT - (qrSize * scale)) / 2;

    for (int y = 0; y < qrSize; y++) {
      for (int x = 0; x < qrSize; x++) {
        if (esp_qrcode_get_module(qr_handle, x, y)) {
          display.fillRect(xOffset + (x * scale), yOffset + (y * scale), scale, scale, SSD1306_WHITE);
        }
      }
    }
  } else {
    Serial.println("QR handle is null!");
  }

  display.display();
}

void loop() {}