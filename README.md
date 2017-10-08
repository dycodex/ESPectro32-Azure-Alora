ESPectro32-Azure-Alora
======================

This is an example project to work with Alora v2 Board and Azure IoT Hub for ESP32-based board. The project is made with `Espressif IoT Development Framework` ([ESP-IDF](https://github.com/espressif/esp-idf)). Please check ESP-IDF docs for getting started instructions.

This project depends on [ESPectro32](https://github.com/dycodex/ESPectro32) library!

## Azure IoT Hub

First thing first, you should have already set up an Azure IoT Hub and add a device. To create your Microsoft Azure IoT Hub and add a device, follow the instructions outlined in the [Setup IoT Hub Microsoft Azure Iot SDK](https://github.com/Azure/azure-iot-device-ecosystem/blob/master/setup_iothub.md) page.

To get started, just take a look the `main.cpp` file. Change the followings:

* `[YOUR_AZURE_IOT_HUB]`
* `[YOUR_DEVICE_ID]`
* `[YOUR_DEVICE_KEY]`

## ESPectro32

The project is tested form [ESPectro32](https://shop.makestro.com/product/espectro32/) board. It is an IoT development board based-on ESP32 MCU, made by [DycodeX](https://dycodex.com).

## Alora v2

An awesome board combines many sensors and actuators. Uncountable use case/project you can make with it. Get one [here](https://shop.makestro.com/product/alorav2/).

## Getting Started

* Clone this repo
* Update submodules which it depends on: `git submodule update --init --recursive` 
* There's a symbolic link `arduino-esp32`. Change (recreate) the symbolic link to point to your local [arduino-esp32](https://github.com/espressif/arduino-esp32) folder.
* There're another two symbolic links: `Alora` and `AzureIoTHub`. Clone [ESPectro32-IDF](https://github.com/dycodex/ESPectro32-IDF), then recreate those two symbolic links to point to respective folder in ESPectro32-IDF folder you just cloned. Don't ask me why for this hassle, I'm too lazy to make them as separate repos :) 
* Try to `make`. Fingers crossed :)
