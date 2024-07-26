void ledControl(String ledColor) {
  const char* _ledColor = ledColor.c_str(); // Convert String to const char*

  if (strcmp(_ledColor, "RED") == 0) {
    ledIndicator.fill(ledIndicator.Color(255, 0, 0), 0, NUMPIXELS); // Red color
  } else if (strcmp(_ledColor, "GREEN") == 0) {
    ledIndicator.fill(ledIndicator.Color(0, 255, 0), 0, NUMPIXELS); // Green color
  } else if (strcmp(_ledColor, "BLUE") == 0) {
    ledIndicator.fill(ledIndicator.Color(0, 0, 255), 0, NUMPIXELS); // Blue color
  } else if (strcmp(_ledColor, "OFF") == 0) {
    ledIndicator.fill(ledIndicator.Color(0, 0, 0), 0, NUMPIXELS); // Off
  }

  ledIndicator.show(); // Send the updated pixel colors to the hardware
}

void ledControlRGB(uint8_t red, uint8_t green, uint8_t blue) {
  ledIndicator.fill(ledIndicator.Color(red, green, blue), 0, NUMPIXELS);
  ledIndicator.show(); // Send the updated pixel colors to the hardware
}


void fingerInit(){
  if (fingerScanner.verifyPassword()) {
    Serial.println(F("Found fingerprint sensor!"));
  } else {
    Serial.println(F("Did not find fingerprint sensor!"));
    while(true){
      delay(1); 
    }
  }

  Serial.println(F("Reading sensor parameters"));
  fingerScanner.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(fingerScanner.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(fingerScanner.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(fingerScanner.capacity);
  Serial.print(F("Security level: ")); Serial.println(fingerScanner.security_level);
  Serial.print(F("Device address: ")); Serial.println(fingerScanner.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(fingerScanner.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(fingerScanner.baud_rate);
  Serial.print(F("Templates: ")); Serial.print(fingerScanner.templateCount);

}



void initFingerID_EEPROM(){
  for(uint8_t x = 1; x < EEPROM_SIZE; x++){
    EEPROM.writeBool(x, ID_AVAILABLE);
  }
  EEPROM.commit();
}

uint8_t getAvailableFingerID(){
  for(uint8_t id = 1; id < EEPROM_SIZE; id++){
    if(EEPROM.readBool(id) == ID_AVAILABLE){
      return id;
    }
  }
  return 255;
}


void printDeposit(String _name, String depositedAmount, String previousTotal, String currentTotal){
  printer.setSize('S');
  printer.justify('C');
  printer.println("\n");
  printer.println("PIGGY BANK SYSTEM");
  printer.println("\n");
  printer.justify('L');
  printer.println("Name: " + String(_name));
  printer.println("Previous Amount: Php " + String(previousTotal) + ".00");
  printer.println("Amount Deposited: Php " + String(depositedAmount) + ".00");
  printer.println("New Account Balance:");
  printer.println("Php " + String(currentTotal) + ".00");
  printer.println("\n");
  printer.println("\n");
}

void printSavings(String _name, String currentTotal){
  printer.setSize('S');
  printer.justify('C');
  printer.println("\n");
  printer.println("PIGGY BANK SYSTEM");
  printer.println("\n");
  printer.justify('L');
  printer.println("Name: " + String(_name));
  printer.println("New Account Balance:");
  printer.println("Php " + String(currentTotal) + ".00");
  printer.println("\n\n");
}

void moneyCounterReset(){
billCounter_1000  = 0;
billCounter_500  = 0;
billCounter_200   = 0;
billCounter_100   = 0;
billCounter_50    = 0;
billCounter_20    = 0;

coinCounter_20  = 0;
coinCounter_10  = 0;
coinCounter_5   = 0;
coinCounter_1   = 0;
}


void ledErrorSignal(byte loopCount){
  for(byte x = 0; x < loopCount; x++){
    ledControlRGB(255, 0, 0);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    ledControlRGB(0, 0, 0);
  }
}

void ledOkSignal(byte loopCount){
  for(byte x = 0; x < loopCount; x++){
    ledControlRGB(0, 255, 0);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    ledControlRGB(0, 0, 0);
  } 
}
