void DeviceStatusTask(void *pvParameters) {
  Serial.print(F("Device Status Monitoring running on core "));
  Serial.println(xPortGetCoreID());

  while (1) {
    switch (currentState) {

      case DEVICE_STATE_INIT:
        if (millis() - ledMillis >= 3000) {
          if (ledStateCheck == 0) {
            ledControlRGB(200, 255, 200);
          }
          else {
            ledControlRGB(255, 255, 255);
          }

          ledStateCheck = !ledStateCheck;
          ledMillis = millis();
        }

        break;

      case DEVICE_STATE_IDLE:
        ledControlRGB(255, 255, 255);

        digitalWrite(BILL_EN_PIN, BILL_DISABLE);
        digitalWrite(COIN_EN_PIN, COIN_DISABLE);
        break;

      case DEVICE_STATE_ACTIVE:
        break;

      case DEVICE_STATE_ERROR:
        break;

      case DEVICE_STATE_MAINTENANCE:
        break;

      case DEVICE_STATE_SLEEP:
        ledControl("OFF");
        break;

      case DEVICE_STATE_FINGER_SCAN:
        ledControlRGB(0, 255, 255);
        break;

      case DEVICE_STATE_DEPOSIT_READY:
        ledControlRGB(0, 0, 255);
        break;

      case DEVICE_STATE_PRINTING:
        ledControlRGB(255, 0, 255);
        break;

      default:
        ledControlRGB(255, 255, 255);
        break;

    }

    doorCurrentState = digitalRead(DOOR_SWITCH);
    
    if(doorPreviousState == DOOR_OPEN && doorCurrentState == DOOR_CLOSE){
      delay(500);
      servo.write(50);
      delay(300);
      servo.write(90);
      doorPreviousState = DOOR_CLOSE;
    }
    
    doorPreviousState = doorCurrentState;

    vTaskDelay(100 / portTICK_PERIOD_MS);
    
  }
}
