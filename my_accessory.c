
#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
	printf("accessory identify\n");
}

// Switch (HAP section 8.38)
// required: ACTIVE
// optional: NAME

// format: bool; HAP section 9.70; write the .setter function to get the switch-event sent from iOS Home APP.
homekit_characteristic_t cha_faucet_active = HOMEKIT_CHARACTERISTIC_(ACTIVE, false);
homekit_characteristic_t cha_faucet_active2 = HOMEKIT_CHARACTERISTIC_(ACTIVE, false);
homekit_characteristic_t cha_faucet_program_mode = HOMEKIT_CHARACTERISTIC_(PROGRAM_MODE, false);
homekit_characteristic_t cha_faucet_program_mode2 = HOMEKIT_CHARACTERISTIC_(PROGRAM_MODE, false);
homekit_characteristic_t cha_faucet_in_use = HOMEKIT_CHARACTERISTIC_(IN_USE, false);
homekit_characteristic_t cha_faucet_in_use2 = HOMEKIT_CHARACTERISTIC_(IN_USE, false);
//homekit_characteristic_t cha_faucet_active3 = HOMEKIT_CHARACTERISTIC_(ACTIVE, false);
//homekit_characteristic_t cha_faucet_active4 = HOMEKIT_CHARACTERISTIC_(ACTIVE, false);

// format: string; HAP section 9.62; max length 64
homekit_characteristic_t cha_name = HOMEKIT_CHARACTERISTIC_(NAME, "Faucet");

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_faucet, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Havevanding"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123456"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
		HOMEKIT_SERVICE(FAUCET, .primary=true, .characteristics=(homekit_characteristic_t*[]){
			&cha_faucet_active,
      &cha_faucet_program_mode,
      &cha_faucet_in_use,
			NULL
		}),
        NULL
    }),
        HOMEKIT_ACCESSORY(.id=2, .category=homekit_accessory_category_faucet, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Haveslangen"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123456"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
    HOMEKIT_SERVICE(FAUCET, .primary=true, .characteristics=(homekit_characteristic_t*[]){
			&cha_faucet_active2,
      &cha_faucet_program_mode2,
      &cha_faucet_in_use2,
      NULL
    }),
        NULL
    }),
    NULL
};

homekit_server_config_t config = {
		.accessories = accessories,
		.password = "111-11-111"
};
