#define NUMPIXELS      15
#define LED_PIN        25
#define BILL_PIN       32
#define COIN_PIN       33
#define COIN_EN_PIN    2
#define BILL_EN_PIN    15
#define SERVO_PIN      26
#define DOOR_LIMIT_PIN 36
#define DOOR_SWITCH    39
#define IR_COIN_PIN    27
bool ledStateCheck = 0;
unsigned int long ledMillis = 0;

int billCounter_1000  = 0;
int billCounter_500  = 0;
int billCounter_200   = 0;
int billCounter_100   = 0;
int billCounter_50    = 0;
int billCounter_20    = 0;

int coinCounter_20  = 0;
int coinCounter_10  = 0;
int coinCounter_5   = 0;
int coinCounter_1   = 0;







enum DeviceState {
  DEVICE_STATE_INIT,
  DEVICE_STATE_IDLE,               // Resting State
  DEVICE_STATE_ACTIVE,
  DEVICE_STATE_ERROR,
  DEVICE_STATE_MAINTENANCE,
  DEVICE_STATE_SLEEP,
  DEVICE_STATE_FINGER_ENROLL,      // When OTP REG Initiated by RPI
  DEVICE_STATE_FINGER_SCAN,        // Login from RPI initiated
  DEVICE_STATE_DEPOSIT_READY,       // TUrn on or Enable Coin Slot and Bill Acceptor
  DEVICE_STATE_PRINTING
};


#define EEPROM_SIZE 64
#define MAX_FINGER_ID 100
#define ID_AVAILABLE   true
#define ID_UNAVAILABLE false

#define COIN_ENABLE   HIGH
#define COIN_DISABLE  LOW
#define BILL_ENABLE   LOW
#define BILL_DISABLE  HIGH

#define DOOR_OPEN  HIGH
#define DOOR_CLOSE LOW

#define BIN_FULL     LOW
#define BIN_NOT_FULL HIGH


#define DEBUG
//#define INIT_EEPROM
