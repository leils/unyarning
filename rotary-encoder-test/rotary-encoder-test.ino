// Rotary Encoder Inputs
#define CLK 2

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

void setup() {

	// Set encoder pins as inputs
	pinMode(CLK,INPUT);
	// Setup Serial Monitor
	Serial.begin(9600);

	// Read the initial state of CLK
	lastStateCLK = digitalRead(CLK);
}

void loop() {

	// Read the current state of CLK
	currentStateCLK = digitalRead(CLK);

	// If last and current state of CLK are different, then pulse occurred
  // I don't care about direction or counting down, I only need to count up
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
			counter ++;
		Serial.println(counter);
	}

	// Remember last CLK state
	lastStateCLK = currentStateCLK;
	// Put in a slight delay to help debounce the reading
	delay(1);
}
