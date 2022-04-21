//-------------------------------------------------------------------//
// WIFI Setup

#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
// test network
char ssid[] = "nestnet";          // your network SSID (name)
char pass[] = "skynetbutwithfeathers";                    // your network password

//destination IP
IPAddress outIp(192, 168, 1, 11); //desktop computer
const unsigned int outPort = 8080;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const unsigned int inPort = 8000;        // local port to listen for UDP packets (here's where we send the packets)
OSCErrorCode error;


//-------------------------------------------------------------------//
// Rotary Encoder Inputs

#define CLK 2

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

void setup() {
	// Setup Serial Monitor
	Serial.begin(9600);

	// Connect to WiFi network
	delay(500);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

	Serial.println("Starting UDP");
  Udp.begin(inPort);
  Serial.print("Local port: ");

  Serial.println(inPort);


	// Set encoder pins as inputs
	pinMode(CLK,INPUT);

	// Read the initial state of CLK
	lastStateCLK = digitalRead(CLK);
}

void sendMessage(int counter) {
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
