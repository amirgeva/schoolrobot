#include "iface.h"

void setup() {
	delay(100);
	Serial.begin(115200);
  delay(100);
  pwm_setup();
  sensor_setup();
	Serial.println();
	Serial.print("Configuring access point...");
  setup_connection();
}

void loop() {
  handle_incoming();
  poll_encoders();
  poll_sensor();
}

