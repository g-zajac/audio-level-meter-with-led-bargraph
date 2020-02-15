<a href="http://gz.technology" target="_blank">![logo](doc/logo.png)</a>

# Audio SPL level meter with led bargraph

This project is about measuring semi-calibrated [SPL](https://en.wikipedia.org/wiki/Sound_pressure#Sound_pressure_level) and displaying the value on 24 pixel led bar.

## About The project

A sound is picked up with an electret microphone, processed and then SPL level calculated and mapped to a led bar.

#### Input

I used Adafruit MAX4466, modified as Pauls suggested in this [thread](https://forum.pjrc.com/threads/40468-Help-with-Basic-Audio-Lib-results?p=126317&viewfull=1#post126317) to match Teensy input levels. The preamp output is connected directly to the Teensy AD input.

#### Microconroller
I have choseen the Teensy platform as this project is focused on audio and power consumption is not so main factor. The [Teensy 3.2](https://www.pjrc.com/teensy/teensy31.html) has got 32bit Cortex-M4 ARM processor running on 72MHz, DMA and 16bit AD covnerter which makes it a perfect for this application IMHO.

#### Output
Neopixel tape is connected via 74HCT245 buffer.

## How It Works
To compute SPL, the microcontroller does:
- converts analog input signal from mic preamp
- applies Hanning window
- calculates 1024 points FFT
- applies A-weighting curve
- compute the SPL using RMS value
- outputs the SPL
- map SPL level to 24 points bargraph

## Schematic
_coming soon_

## Built With
* [Atom](https://atom.io) - An amazing text editor
* [PlatformIO](https://platformio.org) - An ecosystem for embedded development
* [Audio System Design Tool for Teensy Audio Library](https://www.pjrc.com/teensy/gui/) - drag and drop tool for quick implementation an sudio system
* [Teensy Loader Application](https://www.pjrc.com/teensy/loader.html) - for uploading firmware to Teensy microcontroler

## Usage / Prerequisites
Build circuit acording to schematic (comming soon once prototype is tested and finalized).

Clone the repository to a local folder
```bash
git clone https://github.com/gregoryZajac/audio-level-meter-with-led-bargraph.git
```
enter the project folder and run command:
```bash
platformio run
```
Project configuration is set in platformio.ini where you can set up variuos environmets and options.

Upload the compiled code to the Teensy, no programmer needed as has got build in USB and bootloader.

## Performance
CPU 65% so far, SPL only no neopixels yet

## Acknowledgments
Big thanks for inspiration and sharing your knowledge and code:
- [@PaulStoffregen](https://github.com/PaulStoffregen)
- Thijs Schrama - PJRC (Teensy) Forum [thread](https://forum.pjrc.com/threads/34371-Teensy-3-2-Sound-Level-Meter-(sound-measurement-device))
## License
This project is licensed under [MIT license](http://opensource.org/licenses/mit-license.php)

## Project status
- Hardware build in progress, prototyping.
- Firmware under development.
