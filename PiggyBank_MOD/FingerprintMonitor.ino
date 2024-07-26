void FingerprintTask(void *pvParameters) {

  while (1) {
    vTaskDelay(100 / portTICK_PERIOD_MS);

    if (currentState == DEVICE_STATE_FINGER_ENROLL) {
      ledControlRGB(0, 255, 255);

      // Get Avaialble ID for Enrollment
      uint8_t fingerId = getAvailableFingerID();

      // Start Enrollment
      while (currentState == DEVICE_STATE_FINGER_ENROLL) {
        getFingerprintEnroll(fingerId);
      }


    }
    else if (currentState == DEVICE_STATE_FINGER_SCAN) {
      ledControlRGB(0, 255, 255);
      // Scan Finger
      if (getFingerprintIDez() != -1) {
        currentState = DEVICE_STATE_IDLE;
      }

    }
  }
}
