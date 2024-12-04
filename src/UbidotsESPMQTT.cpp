/*
Copyright (c) 2021 Ubidots.
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
Created by: Jose Garcia
*/

/**************************************************************************
 * Overloaded constructors
 ***************************************************************************/

#include "UbidotsESPMQTT.h"

Ubidots::Ubidots(char* token) {
  String _deviceMac = WiFi.macAddress();
  _clientName = new char[_deviceMac.length() + 1];
  strcpy(_clientName, _deviceMac.c_str());
  initialize(token, _clientName);
}

Ubidots::Ubidots(char* token, char* clientName) { initialize(token, clientName); }

/***************************************************************************
FUNCTIONS TO SEND/RETRIEVE DATA
***************************************************************************/

bool Ubidots::ubidotsSubscribe(char* deviceLabel, char* variableLabel) {
  char topic[150];
  sprintf(topic, "%s%s/%s/lv", FIRST_PART_TOPIC, deviceLabel, variableLabel);
  if (!_client.connected()) {
    reconnect();
  }
  if (_debug) {
    Serial.println("Subscribed to: ");
    Serial.println(topic);
  }
  return _client.subscribe(topic);
}

bool Ubidots::ubidotsPublish(char* deviceLabel) {
  char topic[150];
  char payload[500];
  String str;
  sprintf(topic, "%s%s", FIRST_PART_TOPIC, deviceLabel);
  sprintf(payload, "{");
  for (int i = 0; i <= currentValue;) {
    str = String((val + i)->_value, 2);
    sprintf(payload, "%s\"%s\": [{\"value\": %s", payload, (val + i)->_variableLabel, str.c_str());
    if ((val + i)->_timestamp != "NULL") {
      sprintf(payload, "%s, \"timestamp\": %s", payload, (val + i)->_timestamp);
    }
    if ((val + i)->_context != "NULL") {
      sprintf(payload, "%s, \"context\": {%s}", payload, (val + i)->_context);
    }
    i++;
    if (i >= currentValue) {
      sprintf(payload, "%s}]}", payload);
      break;
    } else {
      sprintf(payload, "%s}], ", payload);
    }
  }
  if (_debug) {
    Serial.println("publishing to TOPIC: ");
    Serial.println(topic);
    Serial.print("JSON dict: ");
    Serial.println(payload);
  }
  currentValue = 0;
  return _client.publish(topic, payload, 512);
}

/***************************************************************************
FUNCTIONS TO MANAGE SOCKET CONNECTION
***************************************************************************/

/**
 * Overloaded connect() methods.
 * Connects to the broker using a custom username and password
 * @arg clientName [Optional] Unique MQTT client id
 * @arg username [Mandatory] MQTT username to be identified by the broker
 * @arg password [Mandatory] MQTT password to be identified by the broker
 */
bool Ubidots::connect() { return connect(_clientName, _token, _token); }
bool Ubidots::connect(const char* username, const char* password) { return connect(_clientName, username, password); }
bool Ubidots::connect(const char* clientName, const char* username, const char* password) {
  bool result = _client.connect(clientName, username, password);
  if (_debug) {
    Serial.println("attempting to connect");
    if (!result) {
      Serial.print("failed, rc=");
      Serial.print(_client.state());
    }
  }
  return result;
}

bool Ubidots::connected() { return _client.connected(); }

/**
 * Maintains the socket connection and sends periodically the keep alive command
 */

bool Ubidots::loop() {
  if (!_client.connected()) {
    reconnect();
  }
  return _client.loop();
}

/**
 * Disconnects gracefully from the broker, closing the socket
 */
void Ubidots::disconnect() { _client.disconnect(); };

/***************************************************************************
AUXILIAR FUNCTIONS
***************************************************************************/

/**
 * Add a value of variable to save
 * @arg variable_label [Mandatory] variable label where the dot will be stored
 * @arg value [Mandatory] Dot value
 * @arg context [optional] Dot context to store. Default NULL
 * @arg dot_timestamp_seconds [optional] Dot timestamp in seconds, usefull for
 * datalogger. Default NULL
 */

bool Ubidots::add(char* variableLabel, float value) { return add(variableLabel, value, "NULL", "NULL"); }

bool Ubidots::add(char* variableLabel, float value, char* context) {
  return add(variableLabel, value, context, "NULL");
}

bool Ubidots::add(char* variableLabel, float value, char* context, char* timestamp) {
  (val + currentValue)->_variableLabel = variableLabel;
  (val + currentValue)->_value = value;
  (val + currentValue)->_context = context;
  (val + currentValue)->_timestamp = timestamp;
  currentValue++;
  if (currentValue > MAX_VALUES) {
    Serial.println(F("You are sending more than the maximum of consecutive variables"));
    currentValue = MAX_VALUES;
  }
  return true;
}

void Ubidots::begin(void (*callback)(char*, uint8_t*, unsigned int)) {
  this->callback = callback;
  Serial.println("Client name is: ");
  Serial.println(_clientName);
  if (strcmp(_clientName, "00:00:00:00:00:00") == 0) {
    String _deviceMac = WiFi.macAddress();
    strcpy(_clientName, _deviceMac.c_str());
    Serial.println("Empty client name, using device MAC as client name");
  } 
  _client.setServer(_server, MQTT_PORT);
  _client.setCallback(callback);
}

void Ubidots::initialize(char* token, char* clientName) {
  _server = SERVER;
  _token = token;
  currentValue = 0;
  val = (Value*)malloc(MAX_VALUES * sizeof(Value));
  _clientName = clientName;
}

void Ubidots::reconnect() {
  while (!_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (_client.connect(_clientName, _token, NULL)) {
      Serial.println("connected");
      break;
    } else {
      Serial.print("failed, rc=");
      Serial.print(_client.state());
      Serial.println(" try again in 3 seconds");
      delay(3000);
    }
  }
}

void Ubidots::setDebug(bool debug) { _debug = debug; }

void Ubidots::ubidotsSetBroker(char* broker) {
  if (_debug) {
    Serial.println("Broker set for Business Account");
  }
  _server = broker;
}

bool Ubidots::wifiConnection(char* ssid, char* pass) {
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
  return true;
}
