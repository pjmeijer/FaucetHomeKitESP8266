/*
 * This code, implementing two faucet switches using ESP8266, was adapted using the following code and Arduino-HomeKit-ESP8266. Thanks Wang Bin!
 * https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266/tree/master
 *
 * switch.ino
 *
 *  Created on: 2020-05-15
 *      Author: Mixiaoxiao (Wang Bin)
 *
 * HAP section 8.38 Switch
 * An accessory contains a switch.
 *
 * This example shows how to:
 * 1. define a switch accessory and its characteristics (in my_accessory.c).
 * 2. get the switch-event sent from iOS Home APP.
 * 3. report the switch value to HomeKit.
 *
 * You should:
 * 1. read and use the Example01_TemperatureSensor with detailed comments
 *    to know the basic concept and usage of this library before other examples。
 * 2. erase the full flash or call homekit_storage_reset() in setup()
 *    to remove the previous HomeKit pairing storage and
 *    enable the pairing with the new accessory of this new HomeKit example.
 */


#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

void setup() {
	Serial.begin(115200);
	wifi_connect(); // in wifi_info.h
	//homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
	my_homekit_setup();
}

void loop() {
	my_homekit_loop();
	delay(10);
}

//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_faucet_active;
extern "C" homekit_characteristic_t cha_faucet_active2;
extern "C" homekit_characteristic_t cha_faucet_program_mode;
extern "C" homekit_characteristic_t cha_faucet_program_mode2;
extern "C" homekit_characteristic_t cha_faucet_in_use;
extern "C" homekit_characteristic_t cha_faucet_in_use2;
//extern "C" homekit_characteristic_t cha_faucet_active3;
//extern "C" homekit_characteristic_t cha_faucet_active4;

static uint32_t next_heap_millis = 0;

#define PIN_SWITCH 4
#define PIN_SWITCH2 5
//#define PIN_SWITCH3 12
//#define PIN_SWITCH4 14

//Called when the switch value is changed by iOS Home APP
void cha_faucet_active_setter(const homekit_value_t value) {
	bool on = value.bool_value;
	cha_faucet_active.value.bool_value = on;	//sync the value
	LOG_D("Faucet: %s", on ? "ACTIVE" : "CLOSED");
	digitalWrite(PIN_SWITCH, on ? HIGH : LOW);
}
void cha_faucet_active_setter2(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_faucet_active2.value.bool_value = on;  //sync the value
  LOG_D("Faucet2: %s", on ? "ACTIVE" : "CLOSED");
  digitalWrite(PIN_SWITCH2, on ? HIGH : LOW);
}
void cha_faucet_program_mode_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_faucet_program_mode.value.bool_value = on;  //sync the value
  LOG_D("Program mode: %s", on ? "ON" : "OFF");
}
void cha_faucet_program_mode2_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_faucet_program_mode2.value.bool_value = on;  //sync the value
  LOG_D("Program mode: %s", on ? "ON" : "OFF");
}
void cha_faucet_in_use_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_faucet_in_use.value.bool_value = on;  //sync the value
  homekit_characteristic_notify(&cha_faucet_in_use, cha_faucet_in_use.value);
  LOG_D("In use: %s", on ? "In use" : "Not in use");
}
void cha_faucet_in_use2_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_faucet_in_use2.value.bool_value = on;  //sync the value
  homekit_characteristic_notify(&cha_faucet_in_use2, cha_faucet_in_use2.value);
  LOG_D("In use: %s", on ? "In use" : "Not in use");
}

//void cha_faucet_active_setter3(const homekit_value_t value) {
//  bool on = value.bool_value;
//  cha_faucet_active2.value.bool_value = on;  //sync the value
//  LOG_D("Switch: %s", on ? "ON" : "OFF");
//  digitalWrite(PIN_SWITCH3, on ? HIGH : LOW));
//}
//void cha_faucet_active_setter4(const homekit_value_t value) {
//  bool on = value.bool_value;
//  cha_faucet_active2.value.bool_value = on;  //sync the value
//  LOG_D("Switch: %s", on ? "ON" : "OFF");
//  digitalWrite(PIN_SWITCH4, on ? HIGH : LOW));
//}

void my_homekit_setup() {
	pinMode(PIN_SWITCH, OUTPUT);
	digitalWrite(PIN_SWITCH, LOW);
  pinMode(PIN_SWITCH2, OUTPUT);
  digitalWrite(PIN_SWITCH2, LOW);
  //pinMode(PIN_SWITCH3, OUTPUT);
  //digitalWrite(PIN_SWITCH3, LOW);
  //pinMode(PIN_SWITCH4, OUTPUT);
  //digitalWrite(PIN_SWITCH4, LOW);

	//Add the .setter function to get the switch-event sent from iOS Home APP.
	//The .setter should be added before arduino_homekit_setup.
	//HomeKit sever uses the .setter_ex internally, see homekit_accessories_init function.
	//Maybe this is a legacy design issue in the original esp-homekit library,
	//and I have no reason to modify this "feature".
	cha_faucet_active.setter = cha_faucet_active_setter;
  cha_faucet_active2.setter = cha_faucet_active_setter2;
  cha_faucet_program_mode.setter = cha_faucet_program_mode_setter;
  cha_faucet_program_mode2.setter =  cha_faucet_program_mode2_setter;
  cha_faucet_in_use.setter = cha_faucet_in_use_setter;
  cha_faucet_in_use2.setter = cha_faucet_in_use2_setter;
  // cha_faucet_active3.setter = cha_faucet_active_setter3;
  // cha_faucet_active4.setter = cha_faucet_active_setter4;
	arduino_homekit_setup(&config);

	//report the switch value to HomeKit if it is changed (e.g. by a physical button)
	//bool switch_is_on = true/false;
	//cha_faucet_active.value.bool_value = switch_is_on;
	//homekit_characteristic_notify(&cha_faucet_active, cha_faucet_active.value);
}

bool checkWifi() {
  static unsigned long lastConnect=-20000; 

  // checking for WIFI connection
  if ((WiFi.status() != WL_CONNECTED) && (millis() - lastConnect >= 20000)) {
    Serial.println("Damned. Got disconnected. Reconnecting to WIFI network");
    WiFi.disconnect();
    WiFi.reconnect();
    lastConnect = millis();
  }
  return WiFi.status();
}
void my_homekit_loop() {
	arduino_homekit_loop();
	const uint32_t t = millis();
	checkWifi();
  
	if (t > next_heap_millis) {
		// show heap info every 2 seconds (5 seconds was too long, not updating correctly)
		next_heap_millis = t + 2 * 1000;

		// Read pins
    bool is_switch_on = digitalRead(PIN_SWITCH);
    bool is_switch2_on = digitalRead(PIN_SWITCH2);

    // Check if the in_use has changed
    if (is_switch_on != cha_faucet_in_use.value.bool_value) {
      cha_faucet_in_use.value.bool_value = is_switch_on;
      LOG_D("Faucet in use: %s", is_switch_on ? "ON" : "OFF");
      homekit_characteristic_notify(&cha_faucet_in_use, cha_faucet_in_use.value);
    }

    // Check if the in_use2 has changed
    if (is_switch2_on != cha_faucet_in_use2.value.bool_value) {
      cha_faucet_in_use2.value.bool_value = is_switch2_on;
      LOG_D("Faucet2 in use: %s", is_switch2_on ? "ON" : "OFF");
      homekit_characteristic_notify(&cha_faucet_in_use2, cha_faucet_in_use2.value);
    }

    // Check if the active state has changed
    if (is_switch_on != cha_faucet_active.value.bool_value) {
      cha_faucet_active.value.bool_value = is_switch_on;
      LOG_D("Faucet changed to: %s", is_switch_on ? "ON" : "OFF");
      homekit_characteristic_notify(&cha_faucet_active, cha_faucet_active.value);
    }
    
    // Check if the active2 state has changed
    if (is_switch2_on != cha_faucet_active2.value.bool_value) {
      cha_faucet_active2.value.bool_value = is_switch2_on;
      LOG_D("Faucet2 changed to: %s", is_switch2_on ? "ON" : "OFF");
      homekit_characteristic_notify(&cha_faucet_active2, cha_faucet_active2.value);
    }

    LOG_D("Free heap: %d, HomeKit clients: %d",
				ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
    LOG_D("PIN_SWITCH %d is in use: %d" ,PIN_SWITCH, cha_faucet_in_use.value.bool_value);
    LOG_D("PIN_SWITCH %d is in use: %d" ,PIN_SWITCH2, cha_faucet_in_use2.value.bool_value);

  }

}
