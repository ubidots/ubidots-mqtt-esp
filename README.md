# ubidots-mqtt-esp

MQTT library for connecting to Ubidots using MQTT protocol and an ESP8266 chip.

## Setup

1. Go to the Arduino IDE, click on Files -> Preferences and enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas
2. Open Boards Manager from Tools -> Board menu and install esp8266 platform (and don’t forget to select your ESP8266 board from Tools > Board menu after installation)
3. Download this library as .zip
4. Now, click on Sketch -> Include Library -> Add .ZIP Library
5. Select the .ZIP file that you have just downloaded and then “Accept” or “Choose”
6. Go to Sketch/Program -> Include Library -> Library Manager and install the PubSubClient library 
6. Close the Arduino IDE and open it again.

## Examples

Please refer to examples folder.
