void UARTMonitorTask(void *pvParameters) {
  while (1) {


    if(isEEPROM_Updated == false){
      Serial.println("{\"PB_START_CHK\":\"START\"}");
      vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    if (Serial.available() > 0) {
      String incomingData = Serial.readStringUntil('\n');
#ifdef DEBUG
      Serial.print(F("Received: "));
      Serial.println(incomingData);
#endif

      if (incomingData.indexOf("FP_REGISTER/EN_REQUEST#") > -1) {
        byte index[2];
        index[0] = incomingData.indexOf('#');
#ifdef DEBUG
        Serial.println("OTP: " + String(incomingData.substring(index[0] + 1)));
#endif
        regOTP = incomingData.substring(index[0] + 1);
        Serial.println("{\"FP_REGISTER\":\"RESPONSE\",\"STATUS\":\"ENABLE\"}");
        currentState = DEVICE_STATE_FINGER_ENROLL;
      }

      else if (incomingData.indexOf("FP_DELETE#") > -1) {
        uint8_t index = incomingData.indexOf('#');
        uint8_t idToDelete = incomingData.substring(index + 1).toInt();
        deleteFingerprint(idToDelete);
        Serial.println("{\"FP_DELETE\":\"RESPONSE\",\"STATUS\":\"SUCCESS\"}");

        EEPROM.writeBool(idToDelete, ID_AVAILABLE);
        EEPROM.commit();

      }

      else if (incomingData.indexOf("FP_LOGIN/EN_REQUEST") > -1) {
        Serial.println("{\"FP_LOGIN\":\"RESPONSE\",\"STATUS\":\"ENABLE\"}");
        currentState = DEVICE_STATE_FINGER_SCAN;
      }

      else if (incomingData.indexOf("EN_DEPOSIT") > -1) {
        Serial.println("{\"EN_DEPOSIT\":\"RESPONSE\",\"STATUS\":\"ENABLE\"}");
        digitalWrite(BILL_EN_PIN, BILL_ENABLE);
        digitalWrite(COIN_EN_PIN, COIN_ENABLE);
        currentState = DEVICE_STATE_DEPOSIT_READY;
      }

      else if (incomingData.indexOf("DIS_DEPOSIT") > -1) {
        Serial.println("{\"DIS_DEPOSIT\":\"RESPONSE\",\"STATUS\":\"DISABLE\"}");

        digitalWrite(BILL_EN_PIN, BILL_DISABLE);
        digitalWrite(COIN_EN_PIN, COIN_DISABLE);
        currentState = DEVICE_STATE_IDLE;
      }

      else if (incomingData.indexOf("DEPOSIT_END#") > -1) {
        digitalWrite(COIN_EN_PIN, COIN_DISABLE);
        
        currentState = DEVICE_STATE_PRINTING;
        ledControlRGB(255, 0, 255);


        uint8_t index[4];
        index[0] = incomingData.indexOf('#');
        index[1] = incomingData.indexOf('#', index[0] + 1);
        index[2] = incomingData.indexOf('#', index[1] + 1);
        index[3] = incomingData.indexOf('#', index[2] + 1);

        String _name =  incomingData.substring(index[0] + 1, index[1]);
        String depositedAmount = incomingData.substring(index[1] + 1, index[2]);
        String previousAmount  = incomingData.substring(index[2] + 1, index[3]);
        String currentAmount   = incomingData.substring(index[3] + 1);

        Serial.println(F("Printing Receipt"));
        printDeposit(_name, depositedAmount, previousAmount, currentAmount);
        Serial.println(F("{\"DEPOSIT_END\":\"RESPONSE\",\"STATUS\":\"SUCCESS\"}"));
        moneyCounterReset();

        currentState = DEVICE_STATE_IDLE;
        ledControlRGB(255, 255, 255);

        }

        else if (incomingData.indexOf("PRINT_SAVINGS#") > -1) {

          currentState = DEVICE_STATE_PRINTING;
          ledControlRGB(255, 0, 255);

          uint8_t index[4];
          index[0] = incomingData.indexOf('#');
          index[1] = incomingData.indexOf('#', index[0] + 1);

          String _name =  incomingData.substring(index[0] + 1, index[1]);
          String currentAmount   = incomingData.substring(index[1] + 1);

          Serial.println(F("Printing Receipt"));
          printSavings(_name, currentAmount);
          Serial.println(F("{\"SAVINGS\":\"RESPONSE\",\"STATUS\":\"SUCCESS\"}"));

          currentState = DEVICE_STATE_IDLE;
          ledControlRGB(255, 255, 255);

        }

        else if (incomingData.indexOf("UNLOCK_BANK") > -1) {
          while (digitalRead(DOOR_LIMIT_PIN) == 1) {
            servo.write(150);
            vTaskDelay(1 / portTICK_PERIOD_MS);
          }
          servo.write(90);
        }

        else if (incomingData.indexOf("COIN_CHK_CAP") > -1) {
          String level = digitalRead(IR_COIN_PIN) == BIN_FULL ? "FULL" : "NOT FULL";
          Serial.println("{\"COIN_CHK_CAP\":\"RESPONSE\",\"STATUS\":\"" + level + "\"}");
        }

        
        else if (incomingData.indexOf("RESEND_DEPOSIT") > -1) {
          Serial.println("{\"NEW_DEPOSIT\":\"READY\",\"QB1000\":\"" + String(billCounter_1000) + "\",\"QB500\":\"" + String(billCounter_500) + "\",\"QB200\":\"" + String(billCounter_200) + "\",\"QB100\":\"" + String(billCounter_100) + "\",\"QB50\":\"" + String(billCounter_50) + "\",\"QB20\":\"" + String(billCounter_20) + "\",\"QC20\":\"" + String(coinCounter_20) + "\",\"QC10\":\"" + String(coinCounter_10) + "\",\"QC5\":\"" + String(coinCounter_5) + "\",\"QC1\":\"" + String(coinCounter_1) + "\"}");
        }

      }
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
