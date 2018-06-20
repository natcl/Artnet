/*
This example will receive multiple universes via Artnet and control a strip of ws2811 leds via
Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel
This example may be copied under the terms of the MIT license, see the LICENSE file for details
*/

#include <Artnet.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>

// Neopixel settings
const int numLeds = 240; // change for your setup
const int channelsPerLed = 3;
const int numberOfChannels = numLeds * channelsPerLed; // Total number of channels you want to receive (1 led = 3 channels)
const byte dataPin = 2;
Adafruit_NeoPixel leds = Adafruit_NeoPixel(numLeds, dataPin, NEO_GRB + NEO_KHZ800);

// Artnet settings
Artnet artnet;
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as 0.
int previousDataLength = 0;

// Change ip and mac address for your setup
byte ip[] = {10, 0, 1, 199};
byte mac[] = {0x04, 0xE9, 0xE5, 0x00, 0x69, 0xEC};
byte broadcast[] = {10, 0, 1, 255};
void setup()
{
  //Serial.begin(115200);
  artnet.begin(mac, ip);
  leds.begin();
  artnet.setBroadcast(broadcast);
  initTest();

  // this will be called for each packet received
  artnet.setArtDmxCallback(onDmxFrame);
  artnet.setArtSyncCallback(onSync);
}

void loop()
{
  // we call the read function inside the loop
  artnet.read();
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data, IPAddress remoteIP)
{
  // read universe and put into the right part of the display buffer
  for (int i = 0; i < length / channelsPerLed; i++)
  {
    int led = i + (universe - startUniverse) * (previousDataLength / channelsPerLed);
    if (led < numLeds) {
      if (channelsPerLed == 4)
        leds.setPixelColor(led, data[i * channelsPerLed], data[i * channelsPerLed + 1], data[i * channelsPerLed + 2], data[i * channelsPerLed + 3]);
      if (channelsPerLed == 3)
        leds.setPixelColor(led, data[i * channelsPerLed], data[i * channelsPerLed + 1], data[i * channelsPerLed + 2]);
    }
  }
  previousDataLength = length;
}

void onSync(IPAddress remoteIP) {
  leds.show();
}

void initTest()
{
  for (int i = 0 ; i < numLeds ; i++)
    leds.setPixelColor(i, 127, 0, 0);
  leds.show();
  delay(500);
  for (int i = 0 ; i < numLeds ; i++)
    leds.setPixelColor(i, 0, 127, 0);
  leds.show();
  delay(500);
  for (int i = 0 ; i < numLeds ; i++)
    leds.setPixelColor(i, 0, 0, 127);
  leds.show();
  delay(500);
  for (int i = 0 ; i < numLeds ; i++)
    leds.setPixelColor(i, 0, 0, 0);
  leds.show();
}
