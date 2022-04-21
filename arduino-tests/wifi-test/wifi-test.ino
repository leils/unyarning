/*
 This example connects to an unencrypted WiFi network.
 Then it prints the MAC address of the WiFi module,
 the IP address obtained, and other network details.

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */
#include <SPI.h>
#include <WiFiNINA.h>

#include <WiFiUdp.h>
#include <OSCMessage.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the WiFi radio's status

//destination IP
IPAddress outIp(192, 168, 1, 11); //desktop computer
const unsigned int outPort = 8080;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const unsigned int inPort = 8000;        // local port to listen for UDP packets (here's where we send the packets)
OSCErrorCode error;

#define CLK 2
int counter = 0;
int currentStateCLK;
int lastStateCLK;

void setup() {
  // PIN SETUP
	// Set encoder pins as inputs
	pinMode(CLK,INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600); //Initialize serial
  // while (!Serial) {
  //   ; // wait for serial port to connect. Needed for native USB port only
  // }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  digitalWrite(LED_BUILTIN, HIGH); //LED_BUILTIN is signal for wifi connection
  printCurrentNet();
  printWifiData();

	Serial.println("Starting UDP");
  Udp.begin(inPort);
  Serial.print("Local port: ");

  Serial.println(inPort);

	// Read the initial state of CLK
	lastStateCLK = digitalRead(CLK);
}

void sendMessage(int counter) { //Send OSC message over WIFI
	OSCMessage outMsg("/count");
	outMsg.add(counter);

	Udp.beginPacket(outIp, outPort);
		outMsg.send(Udp); // send the bytes to the SLIP stream
	Udp.endPacket(); // mark the end of the OSC Packet
	outMsg.empty(); // free space occupied by message
}

void loop() {
  	// Read the current state of CLK
	currentStateCLK = digitalRead(CLK);

	// If last and current state of CLK are different, then pulse occurred
  // I don't care about direction or counting down, I only need to count up
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		counter ++;
		sendMessage(counter);
		Serial.println(counter);
	}

	// Remember last CLK state
	lastStateCLK = currentStateCLK;
	// Put in a slight delay to help debounce the reading
	delay(1);

}

void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
