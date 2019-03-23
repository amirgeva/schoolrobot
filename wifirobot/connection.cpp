#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "iface.h"

/* Set these to your desired credentials. */
byte mac[6];
char mac_str[32];
const char *password = "wheatbot";

WiFiUDP Udp;

const int RECV_PORT=33146;
const int IN_BUFFER_SIZE=32;
char incoming[IN_BUFFER_SIZE];


void generate_mac_str()
{
  // convert mac bytes into readable string
  WiFi.macAddress(mac);
  char* ptr=mac_str;
  for(int i=0;i<6;++i)
  {
    if (i>0) *ptr++ = '-';
    sprintf(ptr,"%02X",mac[i]);
    ptr+=2;
  }
  *ptr=0;
}

void setup_connection()
{
  generate_mac_str();
  Serial.print("\nMAC: ");
  Serial.println(mac_str);

  WiFi.softAP(mac_str, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  delay(500);
  Udp.begin(RECV_PORT);
}

void send_message(const char* value)
{
  Udp.beginPacket(Udp.remoteIP(), RECV_PORT);
  Udp.write(value);
  Udp.endPacket();
}

bool handle_incoming()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    int len = Udp.read(incoming, IN_BUFFER_SIZE);
    if (len > 0)
    {
      incoming[len] = 0;
      Serial.print(">");
      Serial.println(incoming);
      const char* response = process_command(incoming);
      if (response)
      {
        Udp.beginPacket(Udp.remoteIP(), RECV_PORT);
        Udp.write(response);
        Udp.endPacket();
      }
    }
  }
  return (packetSize>0);
}

