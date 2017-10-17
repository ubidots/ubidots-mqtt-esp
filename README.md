# ubidots-mqtt-esp

MQTT library for connecting to Ubidots using MQTT protocol and an ESP8266 chip.

## Setup

1. Go to the Arduino IDE, click on Files -> Preferences and enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas
2. Open Boards Manager from Tools -> Board menu and install esp8266 platform (and don’t forget to select your ESP8266 board from Tools > Board menu after installation)
3. Download this library as .zip
4. Now, click on Sketch -> Include Library -> Add .ZIP Library
5. Select the .ZIP file that you have just downloaded and then “Accept” or “Choose”
6. Go to Sketch/Program -> Include Library -> Library Manager and install the PubSubClient library 
7. Close the Arduino IDE and open it again.
8. If you are using windows, please install the appropiate driver for your ESP board (CH340G if you are using a LoLin board or CP2102 if you are using an AMICA board)

# Documentation

## Constructor

### Ubidots

```
Ubidots(char* token)
Ubidots(char* token, char* clientName)
```
> Creates an Ubidots instance, you must setup as input your Ubidots TOKEN, the MQTT client name is optional and must be unique so we recommend you to insert random ASCII characters if you decide to use it, if you don't pass the clientName as parameter to the constructor the library will try to get the MAC of the device as default client name.

## Methods

### Ubidots

```
add(char* variableLabel, float value, char *context, char *timestamp);
```
> Add a variable with a value, context and timestamp to be sent to a certain data source, once you use add() you can publish your variable using the ubidotsPublish() method. You can add 5 variables maximum before of publish them. 
**Important:** As this library depends on a Pubsubclient client, the max length of the JSON dictionary to send by default is 128 bytes, if you want to publish more than 3 variables and they have context or long names you should set at PubSubclient.h the MQTT_MAX_PACKET_SIZE to 512, you can see on your serial console the dictionary to POST if you call the ```setDebug(bool debug)``` method and pass a true value to it. For more information, refer to the PubSubclient official library: https://github.com/knolleary/pubsubclient
```
begin(void (*callback)(char*,uint8_t*,unsigned int));
```

> Sets the callback function for subscribed topics

```
connected();
```
> Returns True if the device is connected to the MQTT broker

```
loop();
```
> Infinite loop for MQTT connection, insert it at the end of your routine

```
reconnect();
```
> For trying to make a reconnection every 5 seconds if the connection is lost.

```
ubidotsSetBroker(char* broker);
```
> Sets the broker properly for publish and subscribe to Ubidots accounts. If your account if a business one, set "business.api.ubidots.com" or the endpoint provided by Ubidots as your broker, see examples for more information.
By default, broker will be set to publish and subscribe to free educational version accounts with broker "things.ubidots.com".
```
setDebug(bool debug);;
```

> Make available debug messages through the serial port.


```
ubidotsPublish(char *deviceLabel);
```
> Publishes the variables added to the specified device label.

```
ubidotsSubscribe(char* deviceLabel, char* variableLabel);
```
> Subscribe to the specified device label and variable label of your Ubidots account.

```
wifiConnection(char* ssid, char* pass);
```
> Connect via Wifi to the specified SSID.

# Examples

Please refer to examples folder.
