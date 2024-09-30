void loop() 
{
  current_state = get_ldr();
  if(current_state && !previous_state)
  {
    startt = millis();
    digitalWrite(LED_PIN, HIGH);
    delay(PERIOD * 1.5);

    // Wait for starting sequence (e.g., 8 bits of '1')
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

          int time_taken = PERIOD * bitss + (bitss / 3) * PERIOD + PERIOD * 2;
          Serial.print("Total time taken: ");
          Serial.print(time_taken);
          Serial.println(" ms");

          float time_in_sec = time_taken * 0.001;
          float speedd = bitss / time_in_sec;
          Serial.print("Speed: ");
          Serial.print(speedd);
          Serial.println(" bits/sec");

          message = "";
          digitalWrite(LED_PIN, LOW);
          break;
      } else if (ch == NULL) {
        break;
      } else {
        message += ch;
      }
    }
  }
  previous_state = current_state;
}

bool detect_start_sequence() {
  String startSequence = "11111111";  // Starting sequence expected from HTML (all '1's)
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
