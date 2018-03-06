#include <Artnet.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>
#include <Adafruit_FeatherOLED_WiFi.h>
#include <Adafruit_NeoPixel.h>

//Neopixels

const int numLeds = 64;
const int numberOfChannels = numLeds * 3;
const byte dataPin = 12;
Adafruit_NeoPixel leds = Adafruit_NeoPixel(numLeds, dataPin, NEO_GRB + NEO_KHZ800);


#include <WiFi101.h>
#include <WiFiUdp.h>

#define VBAT_ENABLED              1
#define VBAT_PIN                  A7
#define FEED_VBAT                 "vbat"

int status = WL_IDLE_STATUS;
char ssid[] = "R&D_wifi"; //  your network SSID (name)
char pass[] = "erede2K16";    // your network password (use for WPA, or use as key for WEP)

Artnet artnet;
Adafruit_FeatherOLED_WiFi oled = Adafruit_FeatherOLED_WiFi();

#if (SSD1306_LCDHEIGHT != 32)
 #error ("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {
        Serial.begin(9600);
        WiFi.setPins(8,7,4,2);
        leds.begin();
        initTest();

        Serial.println("OLED FeatherWing test");

        oled.init();
        oled.clearDisplay();

        pinMode(VBAT_PIN, INPUT);
        oled.setBatteryIcon(true);
        updateVbat();

        oled.refreshIcons();
        oled.clearMsgArea();
        oled.println("Connecting to ...");
        oled.println(ssid);
        // oled.display();

        // while (!Serial) {
        //         ; // wait for serial port to connect. Needed for native USB port only
        // }

        // check for the presence of the shield:
        if (WiFi.status() == WL_NO_SHIELD) {
                Serial.println("WiFi shield not present");
                // don't continue:
                while (true) ;
        }

        // attempt to connect to Wifi network:
        while ( status != WL_CONNECTED) {
                Serial.print("Attempting to connect to SSID: ");
                Serial.println(ssid);
                // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
                status = WiFi.begin(ssid, pass);

                // wait 10 seconds for connection:
                delay(10000);
        }
        Serial.println("Connected to wifi");

        printWifiStatus();
        artnet.begin();
        artnet.setArtDmxCallback(onDmxFrame);

        Serial.println("\nStarting connection to server...");

        //Udp.begin(localPort);

        delay(1000);
}


void loop() {
        artnet.read();

        // if ( VBAT_ENABLED )
        //   updateVbat();

        //long rssi = WiFi.RSSI();
        // oled.setRSSI(rssi);
        // oled.refreshIcons();
        // oled.display();

}

void printWifiStatus() {
        // print the SSID of the network you're attached to:
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());

        // print your WiFi shield's IP address:
        IPAddress ip = WiFi.localIP();
        Serial.print("IP Address: ");
        Serial.println(ip);

        // print the received signal strength:
        long rssi = WiFi.RSSI();
        Serial.print("signal strength (RSSI):");
        Serial.print(rssi);
        Serial.println(" dBm");

        oled.setConnected(true);
        oled.setRSSI(rssi);
        oled.setIPAddress(ip);
        oled.refreshIcons();
        oled.clearMsgArea();
        // oled.display();
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data, IPAddress remoteIP)
{
        // print out our data
        Serial.print("universe number = ");
        Serial.print(universe);
        Serial.print("\tdata length = ");
        Serial.print(length);
        Serial.print("\tsequence n0. = ");
        Serial.println(sequence);
        Serial.print("DMX data: ");
        for (int i = 0; i < length; i++)
        {
                Serial.print(data[i]);
                Serial.print("  ");
        }
        Serial.println();
        Serial.println();

        for (int i = 0; i < (length / 3); i++)  {
                leds.setPixelColor(i, data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
        }
        leds.show();


        // oled.clearMsgArea();
        // oled.print("Universe: ");
        // oled.println(universe);
        // for (int i = 0; i < length ; i++) {
        //   if (i < 5) {
        //     oled.print(data[i]);
        //     oled.print(" ");
        //   }
        // }
        // oled.display();
}

void updateVbat()
{
        int vbatADC   = 0;     // The raw ADC value off the voltage div
        float vbatFloat = 0.0F; // The ADC equivalent in millivolts
        float vbatLSB   = 0.80566F;// mV per LSB

        // Read the analog in value:
        vbatADC = analogRead(VBAT_PIN);
        vbatADC = analogRead(VBAT_PIN);

        // Multiply the ADC by mV per LSB, and then
        // double the output to compensate for the
        // 10K+10K voltage divider
        vbatFloat = ((float)vbatADC * vbatLSB) * 2.0F;

        oled.setBattery(vbatFloat/1000);

}

void initTest()
{
        for (int i = 0; i < numLeds; i++)
                leds.setPixelColor(i, 127, 0, 0);
        leds.show();
        delay(500);
        for (int i = 0; i < numLeds; i++)
                leds.setPixelColor(i, 0, 127, 0);
        leds.show();
        delay(500);
        for (int i = 0; i < numLeds; i++)
                leds.setPixelColor(i, 0, 0, 127);
        leds.show();
        delay(500);
        for (int i = 0; i < numLeds; i++)
                leds.setPixelColor(i, 0, 0, 0);
        leds.show();
}
