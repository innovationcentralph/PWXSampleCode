void MoneyMonitorTask(void *pvParameters) {
  uint32_t coinInsertMillis = 0;
  bool isCoinInserted = false;

  uint32_t billInsertMillis = 0;
  bool isBillInserted = false;

  while (1) {
    vTaskDelay(5 / portTICK_PERIOD_MS);
    if (currentState == DEVICE_STATE_DEPOSIT_READY){
  

      isCoinInserted == true ? digitalWrite(COIN_EN_PIN, COIN_DISABLE) : digitalWrite(COIN_EN_PIN, COIN_ENABLE);
      if (digitalRead(COIN_PIN) == 0) {
        if (isCoinInserted == false) {
          isCoinInserted = true;
        }
        while (digitalRead(COIN_PIN) == 0) {} // Wait for pin to return to HIGH state
        coinCount++;
        coinInsertMillis = millis();

      }
      if (isCoinInserted == true) {
        if (millis() - coinInsertMillis > 500) {
          if (coinCount > 0 && coinCount <= 2) {
            coinCounter_1++;
          }
          else if (coinCount >= 3 && coinCount <= 6) {
            coinCounter_5++;
          }
          else if (coinCount >= 7 && coinCount <= 11) {
            coinCounter_10++;
          }
          else if (coinCount >= 15 && coinCount <= 21) {
            coinCounter_20++;
          }
          Serial.println("COIN COUNT: " + String(coinCount));
          coinCount = 0;
          isCoinInserted = false;
          Serial.println("{\"NEW_DEPOSIT\":\"READY\",\"QB1000\":\"" + String(billCounter_1000) + "\",\"QB500\":\"" + String(billCounter_500) + "\",\"QB200\":\"" + String(billCounter_200) + "\",\"QB100\":\"" + String(billCounter_100) + "\",\"QB50\":\"" + String(billCounter_50) + "\",\"QB20\":\"" + String(billCounter_20) + "\",\"QC20\":\"" + String(coinCounter_20) + "\",\"QC10\":\"" + String(coinCounter_10) + "\",\"QC5\":\"" + String(coinCounter_5) + "\",\"QC1\":\"" + String(coinCounter_1) + "\"}");
        }

      }


      isBillInserted == true ? digitalWrite(BILL_EN_PIN, BILL_DISABLE) : digitalWrite(BILL_EN_PIN, BILL_ENABLE);
      if (digitalRead(BILL_PIN) == 0) {
        if (isBillInserted == false) {
          isBillInserted = true;
        }
        while (digitalRead(BILL_PIN) == 0) {} // Wait for pin to return to HIGH state
        billCount++;
        billInsertMillis = millis();

      }
      if (isBillInserted == true) {
        if (millis() - billInsertMillis > 500) {
          if (billCount > 0 && billCount <= 2) {
            billCounter_20++;
          }
          else if (billCount >= 3 && billCount <= 6) {
            billCounter_50++;;
          }
          else if (billCount >= 7 && billCount <= 11) {
            billCounter_100++;
          }
          else if (billCount >= 15 && billCount <= 21) {
           billCounter_200++;
          }
          else if (billCount >= 45 && billCount <= 51) {
            billCounter_500++;
          }
          else if (billCount >= 95 && billCount <= 101) {
            billCounter_1000++;
          }
          Serial.println("BILL COUNT: " + String(billCount));
          billCount = 0;
          isBillInserted = false;
          Serial.println("{\"NEW_DEPOSIT\":\"READY\",\"QB1000\":\"" + String(billCounter_1000) + "\",\"QB500\":\"" + String(billCounter_500) + "\",\"QB200\":\"" + String(billCounter_200) + "\",\"QB100\":\"" + String(billCounter_100) + "\",\"QB50\":\"" + String(billCounter_50) + "\",\"QB20\":\"" + String(billCounter_20) + "\",\"QC20\":\"" + String(coinCounter_20) + "\",\"QC10\":\"" + String(coinCounter_10) + "\",\"QC5\":\"" + String(coinCounter_5) + "\",\"QC1\":\"" + String(coinCounter_1) + "\"}");

        }

      }


    }
  }
}
