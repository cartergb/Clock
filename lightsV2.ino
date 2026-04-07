#include <Adafruit_NeoPixel.h>

#define LIGHT 5    // Data pin connected to NeoPixel
#define NUMPIXELS  24     

Adafruit_NeoPixel pixels(NUMPIXELS, LIGHT, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
}

void loop() {
  // PULSE UP: 0 to 127 (half brightness) over 5 seconds
  // 5000ms / 127 steps = ~39ms delay per step
  for (int b = 0; b <= 127; b++) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(b, b, b)); // White = equal R, G, B
    }
    pixels.show();
    delay(39); 
  }

  // PULSE DOWN: 127 back to 0 over 5 seconds
  for (int b = 127; b >= 0; b--) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(b, b, b));
    }
    pixels.show();
    delay(39);
  }
}