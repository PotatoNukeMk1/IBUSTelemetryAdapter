# Hobbywing Telemetry Adapter for Flysky i.bus

Uses flysky i.bus protocol to show telemetry data of a hobbywing XeRun or EzRun ESC (RPM, external voltage, ESC temperature, Motor temperature) on your flysky transmitter. Just connect Serial1 to one of the N-Ports of your flysky receiver and Serial2 to the programmer port of your ESC.

The adapter is powered by the receiver. Be sure the BEC voltage of your ESC is not higher then 6V.

 
### What you need:

* [Adafruit Qt Py M0](https://www.adafruit.com/product/4600) (Seeed Studio XIAO SAMD21 maybe also work)
* 2x 3pin DuPont connector and cable or precrimped servo cable
* Shrink tube 15mm
* 1k ohm resistor
* 1N4142 diode
* [Firmware](https://github.com/PotatoNukeMk1/IBUSTelemetryAdapter/releases)


### Works with (tested):

* [XeRun XR10 Pro Elite G2S](https://www.hobbywing.com/en/products/xerun-xr10-pro-g2s39.html)
* [XeRun XR10 G3S](https://www.hobbywing.com/en/products/xerunxr1-justockg3s.html)
* [Flysky Noble NB4 Pro](https://www.flysky-cn.com/noble-pro-description-2) (but NB4 and NB4+ should also work)
* [Flysky Paladin PL18 EV](https://www.flysky-cn.com/paladin-evdescription)


### Works with (untested):

* [XeRun XR10 STOCK SPEC G2](https://www.hobbywing.com/en/products/xerunxr10stockspecg2.html)
* [XeRun XR8 PRO G3](https://www.hobbywing.com/en/products/xerun-xr8-pro-g3282.html)
* [XeRun XR8 Plus G2S](https://www.hobbywing.com/en/products/xerun-xr8-plus-g2s40.html)
* [XeRun XD10 Pro](https://www.hobbywing.com/en/products/xerun-xd10-pro41.html)
* Most of EzRun which support real time data over OTA programmer


### Dont works with:

* XeRun XR10 Stock Spec (No real time data on programming port!)
* XeRun XR10 Pro G2 (No real time data on programming port!)


## Build instructions

![schematic](https://github.com/user-attachments/assets/3263b8b1-3788-4ecf-833a-0b9eef1b962b)

Diode is to prevent backpowering to ESC if board is connected to USB.
Resistor is necessary for the half duplex protocol using single line serial.


## Flash firmware

Connect adapter to your PC. Press reset twice to start USB flashing mode. Then copy the file to the drive that just appeared.


## Credits

Thanks to
* [Adafruit Industries](https://github.com/adafruit/) (Adafruit NeoPixel, Adafruit ZeroTimer)
* [Rob Tillaart](https://github.com/robtillaart/) (CRC)
* [Bart Mellink](https://github.com/bmellink/) (IBusBM)


## Copyright

Copyright (c) 2024 Ronald Schmid
