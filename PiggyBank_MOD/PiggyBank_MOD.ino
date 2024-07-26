#include <Adafruit_NeoPixel.h>
#include <Adafruit_Fingerprint.h>
#include <Adafruit_Thermal.h>
#include <EEPROM.h>
#include <ESP32Servo.h>

#include "SystemConfig.h"
#include "UserConfig.h"

TaskHandle_t hUARTMonitorTask;
TaskHandle_t hDeviceStatusTask;
TaskHandle_t hFingerprintTask;
TaskHandle_t hMoneyMonitorTask;

Adafruit_NeoPixel ledIndicator(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_Fingerprint fingerScanner = Adafruit_Fingerprint(&Serial2);
Adafruit_Thermal printer(&Serial1);
Servo servo;


void UARTMonitorTask(void *pvParameters);
void DeviceStatusTask(void *pvParameters);
void FingerprintTask(void *pvParameters);
void MoneyMonitorTask(void *pvParameters);


void ledControl(String ledColor);
void ledControlRGB(uint8_t red, uint8_t green, uint8_t blue);

DeviceState currentState = DEVICE_STATE_INIT;
String regOTP = "";
uint16_t coinCount = 0;
uint16_t billCount = 0;
bool doorCurrentState;
bool doorPreviousState;
bool isEEPROM_Updated = false;


void setup() {
  
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("System Start"));

  pinMode(BILL_PIN, INPUT_PULLUP);
  pinMode(COIN_PIN, INPUT_PULLUP);
  pinMode(COIN_EN_PIN, OUTPUT);
  pinMode(BILL_EN_PIN, OUTPUT);
  pinMode(DOOR_LIMIT_PIN, INPUT);
  pinMode(DOOR_SWITCH, INPUT);
  pinMode(IR_COIN_PIN, INPUT);

  doorPreviousState = digitalRead(DOOR_SWITCH);

  digitalWrite(BILL_EN_PIN, BILL_DISABLE);
  digitalWrite(COIN_EN_PIN, COIN_DISABLE);

  EEPROM.begin(EEPROM_SIZE);

  #ifdef INIT_EEPROM
    initFingerID_EEPROM();
  #endif

  ledIndicator.begin();
  fingerScanner.begin(57600);
  servo.attach(SERVO_PIN);

  currentState = DEVICE_STATE_IDLE;
  
  Serial1.begin(9600);
  printer.begin();
  
  fingerInit();
  
  delay(500);

  
  uint32_t initEEPROMMillis = millis();
  while(isEEPROM_Updated == false){

        if(millis() - initEEPROMMillis > 5000){
          initEEPROMMillis = millis();
          Serial.println("{\"PB_START_CHK\":\"START\"}");
        }
    
        if (Serial.available() > 0) {
        
          String incomingData = Serial.readStringUntil('\n');
          
    #ifdef DEBUG
          Serial.print(F("Received Setup: "));
          Serial.println(incomingData);
    #endif
    
          if(incomingData.indexOf("PB_START_CHK_FR_WEB#") > -1){
          
            uint8_t numCount = 0;
            
            for (uint8_t i = 0; i < incomingData.length(); i++) {
              if (incomingData.charAt(i) == ',') {
                numCount++;
              }
            }
    
            uint8_t* fingerIDs = (uint8_t*)malloc(numCount * sizeof(uint8_t));
    
            String slicedString = incomingData.substring(incomingData.indexOf('{') + 1);
            
    
            for(uint8_t x = 0; x < numCount; x++){
              fingerIDs[x] = slicedString.substring(0, slicedString.indexOf(',')).toInt();
              slicedString = slicedString.substring(slicedString.indexOf(',') + 1);
            }
    
            // Print IDs
            Serial.println("FINGER ID: ");
            for(uint8_t x = 0; x < numCount; x++){
              Serial.println(String(fingerIDs[x]));
            }
    
            initFingerID_EEPROM();
    
            for(uint8_t x = 0; x < numCount; x++){
              EEPROM.writeBool(fingerIDs[x], ID_UNAVAILABLE);
            }
            EEPROM.commit();

            for(uint8_t x = 1; x < 10; x++){
              String status = EEPROM.readBool(x) ? "AVAILABLE" : "UNAVAILABLE";
              Serial.println("ID "  + String(x) + ": " + status);
            }
    
            isEEPROM_Updated = true;
        
            free(fingerIDs);
               
          }
      }
  }
    Serial.println("STARTING OTHER ROUTINES");
    xTaskCreatePinnedToCore(UARTMonitorTask,   "UART Monitor",     2048, NULL, 3, &hUARTMonitorTask,  0);
    delay(500);
    xTaskCreatePinnedToCore(DeviceStatusTask,  "Devie Status",     2048, NULL, 1, &hDeviceStatusTask, 0);
    delay(500);
    xTaskCreatePinnedToCore(FingerprintTask,   "Finger Access",    2048, NULL, 2, &hFingerprintTask,  0);
    delay(500);
    xTaskCreatePinnedToCore(MoneyMonitorTask,  "Money Monitor",    2048, NULL, 1, &hFingerprintTask,  1);
    delay(500);
  
  
  
}

void loop() {

}
