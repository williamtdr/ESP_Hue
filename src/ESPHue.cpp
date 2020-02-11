/**************************************************************************
				= Philips Hue Library for ESP8266 =

	This library is designed to access and control Philips Hue Lights Directly.

    Written by: Richard Wardlow @ Circuits for Fun, LLC
    GNU GPL, include above text in redistribution
***************************************************************************/

#include <ESPHue.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

////////////////////////////////////////
// ESPHue Class Methods
////////////////////////////////////////
StaticJsonDocument<1500> doc;
ESPHue::ESPHue(WiFiClient& client, const char* APIKey, const char* host, uint8_t port)
{
	_client = &client;
	_apiKey = APIKey;
	_host = host;
	_port = port;

}

void ESPHue::setAPIKey(const char* APIKey)
{
	_apiKey = APIKey;
}

void ESPHue::setHubIP(const char* host)
{
	_host = host;
}

void ESPHue::setHubPort(uint8_t port)
{
	_port = port;
}

String ESPHue::getLightInfo(byte lightNum)
{
  HTTPClient http;
  String url = "http://" + String(_host) + "/api/" + String(_apiKey) + "/lights/" + lightNum;
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode <= 0)
    return "";
  String payload = http.getString();
  Serial.println(payload);
  http.end();
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return "";
  }
  return "";
}

int ESPHue::getLightState(byte lightNum)
{
    String response = getLightInfo(lightNum);
    return (bool)doc["state"]["on"];
}

void ESPHue::setLight(byte lightNum, byte state, byte sat, byte bri, unsigned int hue)
{
  StaticJsonDocument<500> json;
  json["sat"] = String(sat);
  json["on"] = (bool)state;
  json["bri"] = String(bri);
  json["hue"] = String(hue);
  String res = "";
  serializeJson(doc, res);
  String url = "/lights/" + String(lightNum) + "/state";
  sendPut(url, res);
}

void ESPHue::setLight(byte lightNum, byte state, byte sat, byte bri, unsigned int hue, unsigned int trans)
{
  StaticJsonDocument<500> json;
  json["sat"] = String(sat);
  json["on"] = (bool)state;
  json["bri"] = String(bri);
  json["hue"] = String(hue);
  json["transitiontime"] = String(trans);
  String res = "";
  serializeJson(doc, res);
  String url = "/lights/" + String(lightNum) + "/state";
  sendPut(url, res);
}

void ESPHue::setLightPower(byte lightNum, byte state)
{
  String url = "/lights/" + String(lightNum) + "/state";
  String cmd = state == 1 ? "{\"on\":true}": "{\"on\":false}";
  sendPut(url, cmd);
}


String ESPHue::getGroupInfo(byte groupNum)
{
  HTTPClient http;
  String url = "http://" + String(_host)  +"/api/" + String(_apiKey) + "/groups/" + groupNum;
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode <= 0)
    return "";
  String payload = http.getString();
  Serial.println(payload);
  http.end();
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return "";
  }
  return "";
}

int ESPHue::getGroupState(byte groupNum)
{
    String response = getGroupInfo(groupNum);
    return (bool)doc["state"]["any_on"];
}

void ESPHue::setGroup(byte groupNum, byte state, byte sat, byte bri, unsigned int hue)
{
  StaticJsonDocument<500> json;
  json["sat"] = String(sat);
  json["on"] = (bool)state;
  json["bri"] = String(bri);
  json["hue"] = String(hue);
  String res = "";
  serializeJson(doc, res);
  String url = "/groups/" + String(groupNum) + "/action";
  sendPut(url, res);
}

void ESPHue::setGroup(byte groupNum, byte state, byte sat, byte bri, unsigned int hue, unsigned int trans)
{
  StaticJsonDocument<500> json;
  json["sat"] = String(sat);
  json["on"] = (bool)state;
  json["bri"] = String(bri);
  json["hue"] = String(hue);
  json["transitiontime"] = String(trans);
  String res = "";
  serializeJson(doc, res);
  String url = "/groups/" + String(groupNum) + "/action";
  sendPut(url, res);
}
void ESPHue::sendPut(String url, String json){
  String _url = "http://" + String(_host)  +"/api/" + String(_apiKey) + url;
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.PUT(json);
  http.end();
}

void ESPHue::setGroupPower(byte groupNum, byte state)
{
  String url = "/groups/" + String(groupNum) + "/action";
  String cmd = state == 1 ? "{\"on\":true}": "{\"on\":false}";
  sendPut(url, cmd);
}
