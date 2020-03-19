/*C++

Author: graylagx2

Name: Digispark_Responsive_LED_Effect_Button_Cycler

Description:        

    This arduino sketch uses the Digispark Attiny85 with the Neopixel library 
    to carry out LED light effects shows on the WS2812B Series LED strip.
    Each click of a button on the INT0 pin to ground will trigger a new
    light effect in the chain. Use the power switch the shutdown

    You can find the picture of this schematic at and its EAGLE files at:
    https://github.com/graylagx2/Digispark_Responsive_LED_Effect_Button_Cycler

Disclaimer:         
    This is open source from https://github.com/graylagx2 and is intended only for educarional
    purposes. Use in a controlled environment only. By using this sofware you agree you are 
    responsible for any laws toy break or damage you cause.                             
*/


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket/Digispark
#endif

#define BUTTON_PIN   2

#define PIXEL_PIN    0  // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 50  // Number of NeoPixels/LED's

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'
}


// Debounce the BUTTON_PIN and wait out the noise
void debounce() {
    unsigned long start = millis();
    while(millis() - start < 20UL) {
        if(digitalRead(BUTTON_PIN) == LOW) {
            start = millis();
        }
    }
}

// Calling the theme functions
void loop() {
    rainbow(20);
    debounce();
    theaterChaseRainbow(50);
    debounce();
}

// Theme Functions

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  while (1) {
    for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
      for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
        // Offset pixel hue by an amount to make one full revolution of the
        // color wheel (range of 65536) along the length of the strip
        // (strip.numPixels() steps):
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
        // optionally add saturation and value (brightness) (each 0 to 255).
        // Here we're using just the single-argument hue variant. The result
        // is passed through strip.gamma32() to provide 'truer' colors
        // before assigning to each pixel:
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
      strip.show();
      delay(wait);
      if(digitalRead(BUTTON_PIN) == LOW) {
        return;
      }
    }
  }
} 

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  while(1) { // Run rainbow infinite
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();
      delay(wait);
      if(digitalRead(BUTTON_PIN) == LOW) {
        return;
      }
    }
  }
}
