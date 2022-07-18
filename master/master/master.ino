#include <Wire.h>

// DEFINES
#define DEBUG
#define SLAVE_BUTTONS_ADDR 9
#define SLAVE_RFID_ADDR 10
#define SLAVE_SONG_ADDR 11

// GLOBALS
byte slaveValue = 0;
const byte portLED = 3;
enum GamePhase { STEP1_RFID, STEP2_BUTTONS, STEP3_DOOR };
GamePhase gamePhase = GamePhase:: STEP1_RFID;
int songNumber = 1;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  pinMode(portLED, OUTPUT);
  slaveValue = 0;
  Serial.println(F("Démarrage du Jeu"));
}

byte readI2C(int address) {
  byte bval;
  Wire.requestFrom(address, 1);
  bval = Wire.read();
  return bval;
}

void loop() {

  //THINGS TO THINK ABOUT
  /*
   * - Create choice between first or second mission
   * - Create reset mode
   * - Handle music with one arduino and song effect with others
   * - Plug DF Mini Player to amp via DAC outputs and Jack cable
   * - Think about adding bypasses for critical Steps
   * 
   */

  // METHODOLOGIE DU JEU
  /*
   * STEP 1
   * Check if 4 RFIDs are good
   * If yes :
   *  - deactivate the magnet
   *  - Play song effect for oppening
   *  - activate lights under mezza
   * 
   * STEP 2
   * Check if buttons are pushed in the correct order
   * If yes :
   *  - Deactivate magnet
   *  - Play song effect
   *  - activate/change lights
   * 
   * 
   */
  switch (gamePhase) {
    case GamePhase::STEP1_RFID :
      Wire.beginTransmission(SLAVE_RFID_ADDR);
      slaveValue = readI2C(SLAVE_RFID_ADDR);
      Serial.println(songNumber);
      if(slaveValue == 1) {
        Wire.beginTransmission(SLAVE_SONG_ADDR);                                              
        Wire.write(songNumber);
        songNumber++;
        Serial.println(F("RFID OK !"));
        gamePhase = GamePhase::STEP2_BUTTONS;
        Wire.endTransmission();
        delay(1000);
      }
      break;
    case GamePhase::STEP2_BUTTONS :
      slaveValue = 0;
      Wire.beginTransmission(SLAVE_BUTTONS_ADDR);
      slaveValue = readI2C(SLAVE_BUTTONS_ADDR);
      if(slaveValue == 1) {
        Serial.println(F("Aimant désactivé !"));
        digitalWrite(portLED, HIGH);
        Wire.beginTransmission(SLAVE_SONG_ADDR);                                              
        Wire.write(songNumber);
        songNumber++;
        gamePhase = GamePhase::STEP3_DOOR;
        Wire.endTransmission();
        delay(1000);
      }
    break;
    case GamePhase::STEP3_DOOR :
      digitalWrite(portLED, LOW);
      Serial.println(F("Phase 3 activée"));
      break;
    default :
      break;
  }
  delay(200);
}
