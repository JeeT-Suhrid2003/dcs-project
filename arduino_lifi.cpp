#include<math.h>
#define LED_PIN 13
#define LDR_PIN A0
#define THRESHOLD 300
#define PERIOD 100

bool previous_state;
bool current_state;

void setup() 
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Hello from Receiver 2!" );
  digitalWrite(LED_PIN, LOW);
}

unsigned int st=millis();
String message = "";
unsigned long startt;

void loop() 
{
  current_state = get_ldr();
  
  // Trigger when LDR state changes (for detecting start of transmission)
  if(current_state && !previous_state)
  {
    startt = millis();
    digitalWrite(LED_PIN, HIGH);
    delay(PERIOD * 1.5);

    // Wait for starting sequence (e.g., 8 bits of '0')
    while (!detect_start_sequence()) {
      // Keep checking for the starting sequence
    }

    char prev_char = 'a';
    while (1) {
      char ch = get_byte();
      if (ch == '\n') {
          Serial.println();
          delay(200);
          Serial.println("Message: " + message);
          int bitss = message.length() * 8;
          Serial.print("Total bits transferred: ");
          Serial.println(bitss);

          // Calculate total time taken including extra delays
          int time_taken = PERIOD * bitss + (bitss / 3) * PERIOD + PERIOD * 2;
          Serial.print("Total time taken: ");
          Serial.print(time_taken);
          Serial.println(" ms");

          // Calculate speed
          float time_in_sec = time_taken * 0.001;
          float speedd = bitss / time_in_sec;
          Serial.print("Speed: ");
          Serial.print(speedd);
          Serial.println(" bits/sec");

          // Reset for the next message
          message = "";
          digitalWrite(LED_PIN, LOW);
          break;
      } else if (ch == NULL) {
        // If no byte is detected, exit loop
        break;
      } else {
        message += ch;
      }
    }
  }
  previous_state = current_state;
}

// Function to detect the start sequence (e.g., 8 bits of '0')
bool detect_start_sequence() {
  String startSequence = "00000000";  // Starting sequence expected from HTML
  String receivedSequence = "";

  // Read 8 bits for the start sequence
  for (int i = 0; i < 8; i++) {
    bool volt = analogRead(LDR_PIN) > THRESHOLD ? false : true;
    receivedSequence += (volt ? '1' : '0');
    delay(PERIOD);
  }

  // Check if the received sequence matches the start sequence
  return receivedSequence == startSequence;
}

// Function to get LDR sensor value
bool get_ldr()
{
  int voltage = analogRead(LDR_PIN);
  return voltage > THRESHOLD ? false : true;
}

// Function to read a byte of data (8 bits) from the LDR
char get_byte()
{
  int res = 0;
  int pov = 128;  // Highest bit value (2^7)

  for(int i = 0; i < 8; i++)
  {
   bool volt = analogRead(LDR_PIN) > THRESHOLD ? false : true;
   Serial.print(volt);
   res += volt * pov;
   pov /= 2;
   delay(PERIOD);  // Wait for the period between bits
  }

  char character = res;
  Serial.print(" -> ");
  Serial.print(character);
  Serial.println();
  
  return res;
}
