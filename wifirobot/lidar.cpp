#include <stdlib.h>
#include "iface.h"
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X sensor = Adafruit_VL53L0X();
bool lidar_on=false;
char lidar_message[16];

void process_lidar_command(int* params, int n)
{
  if (n==1) lidar_on=(params[0] != 0);
}

void sensor_setup()
{
  if (!sensor.begin()) 
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
}

void poll_sensor()
{
  if (lidar_on)
  {
    VL53L0X_RangingMeasurementData_t measure;
    sensor.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      sprintf(lidar_message,"%d",int(measure.RangeMilliMeter));
      send_message(lidar_message);
    } else {
      Serial.println(" out of range ");
      send_message("-1");
    }
  }
}
