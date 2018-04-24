/*
This is an adaptation from ArtnetReceiveCallback
Here we subscribe to a particular address and amount of channels
and register a callback for that data alone.
This example may be copied under the terms of the MIT license, see the LICENSE file for details
*/

#include <Artnet.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>

Artnet artnet;

// Create a fixture buffer
Patch patch;		// We will "patch" a buffer into the artnet object

// Change ip and mac address for your setup
byte ip[] = {192, 168, 2, 2};
byte mac[] = {0x04, 0xE9, 0xE5, 0x00, 0x69, 0xEC};

void setup()
{
  Serial.begin(115200);
  artnet.begin(mac, ip);

  patch.startAddress =  1; // Maybe unintuive, but remember dmx addresses count from 1
  patch.universe      = 0;
  patch.net           = 0;
  patch.subnet        = 0;
  patch.channels      = 3; // ie say a single RGB fixture

  // this will be called for each packet received
  artnet.patch_to(patch);
  // and subscribe to events as usual
  artnet.setArtFixtureCallback(onMyFrame);
}

void loop()
{
  // we call the read function inside the loop
  artnet.read();
}

void onMyFrame(uint16_t length, uint8_t sequence, uint8_t* data, IPAddress remoteIP)
{
  // The callback will work just the same, but return only the date we subscribed to
  // print out our data
  Serial.print("Packet sequence = ");
  Serial.println(sequence);

  Serial.print("DMX data: ");
  for (int i = 0 ; i < length ; i++)
  {
    Serial.print(data[i]);
    Serial.print("  ");
  }
  Serial.println();
  Serial.println();
}
