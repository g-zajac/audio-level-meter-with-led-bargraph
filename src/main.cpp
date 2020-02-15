#include <Arduino.h>

// ------------------------------ Neopixels ------------------------------------
#include <FastLED.h>
#define NUM_LEDS 24
#define DATA_PIN 6
CRGB leds[NUM_LEDS];
int brightness = 10; // 0-255

// -------------------------   A-weight coefficients ---------------------------
// for fft 1024 the frequency resolution is 43Hz, 43 * 512 = 20016Hz
// Coefficients are calculated in a python script based on formula: https://en.wikipedia.org/wiki/A-weighting
// to run the script: python a-weight/a-curve-gen.py > src/acurve.h
#include "acurve.h"


// code genrated with Audio System Design Tool for Teensy Audio Library
// www.pjrc.com/teensy/gui

#include <Audio.h>
// NOTE are the libraries below needed?
// #include <Wire.h>
// #include <SPI.h>
// #include <SD.h>
// #include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=320,151
AudioAnalyzeFFT1024      fft1024_1;      //xy=496,151
AudioConnection          patchCord1(adc1, fft1024_1);
// GUItool: end automatically generated code

// ---- end of code generated by the Audio System Design Tool

// AudioConnection objects do not have any functions.
// They are simply created in the sketch, after the audio objects define the data flow between those objects.

// some variables
float magnitude = 0;
float dB;

unsigned long previousMillis = 0;
unsigned long samplingInterval = 100;  //in ms
unsigned long previousMillis_monitoring = 0;
unsigned long monitoringInterval = 5 * 1000;  // every 5 secs


void setup() {
  // put your setup code here, to run once:

  // ADC resolution defaults to 10 bits for Arduino compatibility.
  // https://www.arduino.cc/reference/en/language/functions/zero-due-mkr-family/analogreadresolution/
  // set to 16bits
  analogReadResolution(16);

  // Allocate the memory for all audio connections.
  // The numberBlocks input specifies how much memory to reserve for audio data. Each block holds 128 audio samples
  AudioMemory(10);  // tested with AudioMemoryUsageMax()

  // Configure the window algorithm to use for avoiding spectral leakage effect
  fft1024_1.windowFunction(AudioWindowHanning1024);

  Serial.begin(115200);

  // set up neopixels
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  //neopixel test
  FastLED.setBrightness(brightness);
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Blue;
  FastLED.show();
}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned long currentMillis_monitoring = millis();
  if(currentMillis_monitoring - previousMillis_monitoring > monitoringInterval) {
    previousMillis_monitoring = currentMillis_monitoring;
  //monitoring system usage
  Serial.print("Max audio mem used: ");
  Serial.print(AudioMemoryUsageMax());
  Serial.print(" cpu usage max: ");
  Serial.println(AudioProcessorUsageMax());
  }


  // ---------  SPL algorithm with A-weight -----------------------
  // - apply window function (e.g. Hann or Hamming)
  // - calculate FFT
  // - calculate magnitude of FFT (sqrt(re*re+im*im))
  // - convert magnitude to dB (20*log10(magnitude))

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > samplingInterval) {
    previousMillis = currentMillis;

    if (fft1024_1.available()) {
      float v[512] = {0};
      magnitude = 0;

      for (int i=0; i<512; i++){
        // The FFT1024 feature in the Teensy audio lib deals only with ordinary real numbers.
        // Internally it's feeding audio data into the FFT as real numbers. The imaginary part of the input is set to zero.
        // The FFT math gives a complex (real+imaginary) number output for each frequency bin.
        // Conceptually, each frequency has an amplitude (or "magnitude" would be the more mathematically correct term)
        // and a phase shift which is relative to the 23.2 ms time period where the analysis was done.
        // That is *why* FFT output must be real+imaginary numbers; you simply can't represent amplitude *and* phase shift with a single number!
        // The audio library's FFT1024 is written with the assumption you're doing music visualization or
        // other spectral analysis where you only care about how intense each frequency bin is, but you couldn't care less
        // about the relative timing or phase shift between each frequency bin. So it combines the real & imaginary numbers
        // into only a single "magnitude" output for each bin. The downside is you can't get the phase info (at least not using the
        //  simple object from the library) but the library is simpler to use for most ordinary projects where the phase info isn't important.
        // FFT has a special property if you give it only real numbers input (the imaginary part of all 1024 inputs are zero)

        v[i] = fft1024_1.read(i) * a_weighting_curve[i]; // Read frequency bins. The result is scaled so 1.0 represents a full scale sine wave.
        // The term bins is related to the result of the FFT, where every element in the result array is a bin.
        // One can say this is the “resolution” of the FFT. Every bin represent a frequency interval, just like a histogram.
        // The number of bins you get is half the amount of samples spanning the frequency range from zero to half the sampling rate

        magnitude = magnitude + sq(v[i]); //
      } // end of for

      magnitude = sqrt(magnitude);
      dB = log10f(magnitude) * 20  + 125.05;  // db = 20(log A/Aref)
      // TODO turn of or make conditional for producion
      Serial.print("db = ");
      Serial.println(dB,2);
    } // end of if fft

  } // end of if millis

}
