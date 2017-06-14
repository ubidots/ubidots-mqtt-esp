/*
Copyright (c) 2016 Ubidots.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Original Maker: Mateo Velez - Metavix for Ubidots Inc
Modified by: Jose Garcia

*/

#ifndef UbidotsESPMQTT_H
#define UbidotsESPMQTT_H
#include "PubSubClient.h"
#include <ESP8266WiFi.h>

#define MQTT_PORT 1883
#define SERVER "things.ubidots.com"
#define MAX_VALUES 5
#define FIRST_PART_TOPIC "/v1.6/devices/"
#define DEFAULT_DEVICE_LABEL "ESP8266"


#define META_DEBUG Serial

typedef struct Value {
    char  *_variableLabel;
    float _value;
    char *_context;
    char *_timestamp;
} Value;



class Ubidots {
 private:
    void (*callback)(char*,uint8_t*,unsigned int);
    char* getMac();
    void initialize(char* token, char* clientName);
    WiFiClient espClient;
    PubSubClient _client = PubSubClient(espClient);
    char* _clientName = NULL;
    bool _debug = false;
    uint8_t currentValue;
    char* _password;
    char* _server;
    char* _ssid;
    char* _token;
    Value * val;
 
 public:
    Ubidots(char* token);
    Ubidots(char* token, char* clientName);
    bool add(char* variableLabel, float value);
    bool add(char* variableLabel, float value, char *context);
    bool add(char* variableLabel, float value, char *context, char *timestamp);
    void begin(void (*callback)(char*,uint8_t*,unsigned int));
    bool connected();
    bool loop();
    bool ubidotsSubscribe(char* deviceLabel, char* variableLabel);
    bool ubidotsPublish(char *deviceLabel);
    void ubidotsSetBroker(char* broker);
    void reconnect();
    void setDebug(bool debug);
    bool wifiConnection(char* ssid, char* pass);
};

#endif
