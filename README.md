# OS for TeensySaber, ProffieBoard 

## Software Configuration

There is a bunch of different ways to setup blade styles, including write new ones. Here are a list of already implemented ones:

* StyleNormalPtr<COLOR, FLASH_COLOR, OUT_MILLIS, IN_MILLIS> - Normal solid-color lightsaber blade.
* StyleFirePtr<COLOR1, COLOR2> - Fire-style lightsaber blade.
* StyleRainBowPtr<OUT_MILLIS, IN_MILLIS> - Rainbow blade
* StyleStrobePtr<FLASH_COLOR, CLASH_COLOR, OUT_MILLIS, IN_MILLIS> - stroboscope style blade
* &style_pov - POV writer (shows the STAR WARS logo if you have a WS2811 or FASTLED blade.)
* &style_charging - battery level monitor

There is also a template-based system which can build up a bladestyle from a bunch of simpler effects. For instance, StyleNormalPtr is actually implemented as:

* StylePtr<InOutHelper<SimpleClash<COLOR, CLASH_COLOR>, OUT_MILLIS, IN_MILLIS>()

Here is a list of individual components that can be used to build a blade style:
* Rgb<R, G, B> - Solid 8-bit color. (Rgb<255,255,255> is white)
* Rgb16<R, G, B> - Solid 16-bit color. (Rgb<65536,65536,65536> is white)
* Gradient<BASE_COLOR, TIP_COLOR> - smooth fade from base to tip
* Rainbow - scrolling RGB rainbow
* Pulsing<COLOR1, COLOR2, pulse_millis> - smoothly translate between COLOR1 and COLOR2 every pulse_millis.
* RandomFlicker<COLOR1, COLOR2> - Randomly mixes between COLOR1 and COLOR2, mix is even over entire blade.
* RandomPerLEDFlicker<COLOR1, COLOR2> - Randomly mixes between COLOR1 and COLOR2, mix is different for each LED.
* AudioFlicker<COLOR1, COLOR2> - Like RandomFlicker, but chooses based on audio, quiet audio means more COLOR1, loud audio means more COLOR2. The choice is made based on a single sample to make it flickery.
* OnSpark<COLOR, SPARK_COLOR, MILLIS< - Normally defaults to "COLOR", but right as you turn on the saber, it uses SPARK_COLOR, then fads back to COLOR.
* SimpleClash<COLOR, CLASH_COLOR, CLASH_MILLIS< - Normally defaults to "COLOR", but when when you whack the saber, it will use CLASH_COLOR for CLASH_MILLIS millseconds.
* Lockup<COLOR, LOCKUP_COLOR> - Normally uses "COLOR", but during lockup, use "LOCKUP_COLOR" instead.
* Strobe<COLOR, STROBE_COLOR, FREQUENCY, STROBE_MILLIS< - Similar to SimpleClash, but flickers at a fixed frequency.
* InOutHelper<COLOR, OUT_MILLIS, IN_MILLIS> - Makes part of the blade black to make in/out/on/off work the way you expect.
* InOutSparkTip<COLOR, OUT_MILLIS, IN_MILLIS, SPARK_COLOR> - Similar to InOutHelper, but as the blade is extended, the part closest to the tip will be SPARK_COLOR. (usually white)
* StylePtr<COLOR> - Converts one of the templates from above to a BladeStylePtr so you can use it in a preset array.

## Debug Guide
### Serial Monitor Commands
Connect using 9600 baud rate
You can use the serial monitor to debug and control the software once uploaded, here is an incomplete list of commands:

| Command        | Description   | Note          |
| -------------  | ------------- | ------------- |
| help | Prints a list of known commands | N/A |
| on | Turns the saber on | N/A |
| off | Turns the saber off | N/A |
| blade on | Turns the blade on (but leaves the sound off) | N/A |
| blade off | Turns the blade off | N/A |
| clash | Trigger a clash effect | N/A |
| lock | Begin/end lockup | N/A |
| dir [directory] | List files on sd card | N/A |
| cd [directory] | Change directory, and sound font | N/A |
| play [file] | Play wav file | N/A | 
| batt[ery[_voltage]] | Show battery voltage | N/A |
| effects | List current effects | N/A |
| version | Show software version | N/A |
| ssd "text" | Prints <Text> on LCD | N/A |
| mon[itor] swings/samples/touch/battery/pwm/clash/temp/serial | Toggle monitoring | N/A |
| reset | Restart software | N/A | 

## Hardware Wiring
### V1 Wiring
![Alt text](./Tutorial/Lightsaber_bb.png?raw=true "V1 Wiring")

### Charging Adapter
![Alt text](./Tutorial/charging_adapter.png?raw=true "Charging Adapter")

### Led Star Blade
![Alt text](./Tutorial/led_star_circuit.png?raw=true "Led Star Blade")

### Adresable Led String Blade
![Alt text](./Tutorial/pl9823_string.png?raw=true "Adresable Led String Blade")