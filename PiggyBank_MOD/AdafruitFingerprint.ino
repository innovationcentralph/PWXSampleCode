uint8_t getFingerprintEnroll(uint8_t id) {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = fingerScanner.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
      case FINGERPRINT_IMAGEFAIL:
      default:
        ledErrorSignal(2);
        Serial.println(F("{\"FP_REGISTER\":\"RESPONSE\",\"STATUS\":\"ERROR\"}"));
        break;
    }
  }

  // OK success!

  p = fingerScanner.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      ledOkSignal(1);
      
      Serial.println("{\"FP_REGISTER\":\"RESPONSE\",\"STATUS\":\"SUCCESS_1\"}");
      break;
    case FINGERPRINT_IMAGEMESS:
    case FINGERPRINT_PACKETRECIEVEERR:
    case FINGERPRINT_FEATUREFAIL:
    case FINGERPRINT_INVALIDIMAGE:
    default:
      ledErrorSignal(2);
      Serial.println(F("{\"FP_REGISTER\":\"RESPONSE\",\"STATUS\":\"ERROR\"}"));
      return p;
  }

  Serial.println(F("Remove finger"));
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = fingerScanner.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println(F("Place same finger again"));
  while (p != FINGERPRINT_OK) {
    p = fingerScanner.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
      case FINGERPRINT_IMAGEFAIL:
      default:
        ledErrorSignal(2);
        Serial.println(F("{\"FP_REGISTER\":\"RESPONSE\",\"STATUS\":\"ERROR\"}"));
        break;
    }
  }

  // OK success!

  p = fingerScanner.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
    case FINGERPRINT_PACKETRECIEVEERR:
    case FINGERPRINT_FEATUREFAIL:
    case FINGERPRINT_INVALIDIMAGE:
    default:
      ledErrorSignal(2);
      Serial.println(F("{\"FP_REGISTER\":\"RESPONSE\",\"STATUS\":\"ERROR\"}"));
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = fingerScanner.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Prints matched!"));
  } else {
    ledErrorSignal(2);
    Serial.println(F("{\"FP_REGISTER\":\"RESPONSE\",\"STATUS\":\"ERROR\"}"));
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = fingerScanner.storeModel(id);
  if (p == FINGERPRINT_OK) {
    ledOkSignal(3);
    Serial.println("{\"FP_REGISTER\":\"RESPONSE\",\"STATUS\":\"SUCCESS_2\",\"OTP\":\"" + regOTP + "\",\"UID\":\"" + String(id) + "\"}");
    EEPROM.writeBool(id, ID_UNAVAILABLE);
    EEPROM.commit();
    currentState = DEVICE_STATE_IDLE;
  } else {
    ledErrorSignal(2);
    Serial.println(F("{\"FP_REGISTER\":\"RESPONSE\",\"STATUS\":\"ERROR\"}"));
    return p;
  }

  return true;
}


uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = fingerScanner.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
  }

  return p;
}


int getFingerprintIDez() {
  uint8_t p = fingerScanner.getImage();
  if (p != FINGERPRINT_OK) {
    //Serial.println(F("{\"FP_LOGIN\":\"RESPONSE\",\"STATUS\":\"FAILED\"}"));
    return -1;
  }

  p = fingerScanner.image2Tz();
  if (p != FINGERPRINT_OK) {
    
    //Serial.println(F("{\"FP_LOGIN\":\"RESPONSE\",\"STATUS\":\"FAILED\"}"));
    return -1;
  }

  p = fingerScanner.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    ledErrorSignal(2);
    Serial.println(F("{\"FP_LOGIN\":\"RESPONSE\",\"STATUS\":\"FAILED\"}"));
    return -1;
  }

  // found a match!
  Serial.print(F("Found ID #")); Serial.print(fingerScanner.fingerID);
  Serial.print(F(" with confidence of ")); Serial.println(fingerScanner.confidence);
  Serial.println("{\"FP_LOGIN\":\"RESPONSE\",\"STATUS\":\"LOGGEDIN\",\"UID\":\"" + String(fingerScanner.fingerID) + "\"}");
  ledOkSignal(3);
  return fingerScanner.fingerID;
}
