#include <Arduino.h>

#define serial_debuging


#define BUILDIN_LED 13


// ------------------------------ Neopixels ------------------------------------
#include <FastLED.h>
#define NUM_STRIPS 2
#define NUM_LEDS 32
#define DATA_PIN_EXTERNAL_LEDS 6
#define DATA_PIN_INTERNAL_LEDS 7

CRGB leds[NUM_LEDS];
// add controllers for separate brightness managment
CLEDController *controllers[NUM_STRIPS];

// initial brightness before updated from pots
uint8_t external_leds_brightness = 10; // 0-255
uint8_t internal_leds_brightness = 10;

// braking bargraph color points [green -> LOW <- orange -> HIGH <- red]
const int bar_brk_point_low = 18;
const int bar_brk_point_high = 28;

int level = 0;            //bargraph level maped from SPL
int levelOnBar = 0;
int peakLevel = 0;
int barSpeed = 10;  // 10ms?

unsigned long peakFadeTime = 3*1000;  // 3 secs ?
unsigned long peakPreviousTime = 0;

// pixel defined colors
// https://github.com/FastLED/FastLED/wiki/Pixel-reference
// Tomato, Maroon. DarkMagenta
#define PEAK_COLOR DarkRed;


// ***************************** FUNCTIONS *************************************

// function displaying a level on neopixel bargraph
void display_on_bar(int newLevel){
  // set boundaries
  if (newLevel <0){ newLevel = 0; }
  if (newLevel > 31) { newLevel = 31; }

  // level going up
  if (newLevel > levelOnBar){
      for (int dot = levelOnBar; dot <= newLevel; dot ++){
        if(dot>=0 && dot < bar_brk_point_low){
          leds[dot] = CRGB::Green;
        }
        else if (dot >=bar_brk_point_low && dot < bar_brk_point_high){
          leds[dot] = CRGB::Orange;
        }
        else if (dot >= bar_brk_point_high){
          leds[dot] = CRGB::Red;
        }
        controllers[0]->showLeds(internal_leds_brightness);
        controllers[1]->showLeds(external_leds_brightness);
        delay(barSpeed);
      }
      levelOnBar = newLevel;

  // level going down
  } else if (newLevel < levelOnBar){
    for (int dot = levelOnBar; dot >= newLevel; dot-- ){
      leds[dot] = CRGB::Black;

      // do not black the peak dot
      leds[peakLevel] = CRGB::PEAK_COLOR;

      controllers[0]->showLeds(internal_leds_brightness);
      controllers[1]->showLeds(external_leds_brightness);
      delay(barSpeed);
    }
    levelOnBar = newLevel;
  } // end of level on bar if

// -----------------  peak dot --------------------

  if (newLevel >= peakLevel){
    peakLevel = newLevel;

    leds[peakLevel] = CRGB::PEAK_COLOR;
    controllers[0]->showLeds(internal_leds_brightness);
    controllers[1]->showLeds(external_leds_brightness);

  } else {
    unsigned long currentPeakTimer = millis();
    if (currentPeakTimer - peakPreviousTime > peakFadeTime) {
      peakPreviousTime = currentPeakTimer;

      if (peakLevel > levelOnBar){
        leds[peakLevel] = CRGB::Black;
        peakLevel--;
      }

      leds[peakLevel] = CRGB::PEAK_COLOR;
      controllers[0]->showLeds(internal_leds_brightness);
      controllers[1]->showLeds(external_leds_brightness);
    }
  }
}



// *************************** END OF FUNCTIONS ********************************

void setup() {
  // put your setup code here, to run once:

  #ifdef serial_debuging
    Serial.begin(115200);
  #endif

  // set up neopixels
  controllers[0] = &FastLED.addLeds<NEOPIXEL, DATA_PIN_INTERNAL_LEDS>(leds, NUM_LEDS);
  controllers[1] = &FastLED.addLeds<WS2811, DATA_PIN_EXTERNAL_LEDS>(leds, NUM_LEDS);

  pinMode(BUILDIN_LED, OUTPUT);

  digitalWrite(BUILDIN_LED, HIGH);
}

void loop() {
  // value invertet because pullup input, HIGH state when dipswitch set @0, LOW when @ 1
    // test mode
    for (int i=0; i<=7; i++){
      display_on_bar(i);
      delay(50);
    }
    delay(1000);
    for (int i=7; i>=0; i--){
      display_on_bar(i);
      delay(55  0);
    }
    delay(1000);
    controllers[0]->showLeds(internal_leds_brightness);
    controllers[1]->showLeds(external_leds_brightness);

}
