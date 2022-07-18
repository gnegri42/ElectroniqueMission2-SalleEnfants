#include <RFID.h>
#include <SPI.h>
#include <Wire.h>


// RFID 1
#define RFID_1_PIN_RST  9
#define RFID_1_PIN_SDA  10
#define SLAVE_RFID_ADDR 10

// DEBUG
const bool DEBUG = true;

// RFID
// Déclaration des lecteurs RFID
RFID lecteurRFID1(RFID_1_PIN_SDA,RFID_1_PIN_RST);
// Création des tableaux des badges
int UID1[5]; // Badge lu par RFID 1
int mesure;
int MASTERKEY[5]={202,216,136,128,26}; // ID Badge valide 1
bool RFID1Checked = false; // Prevent RFID1 search if one is already scanned

// State of RFID to send in answer
byte rfidState = 0; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // initialize serial
  SPI.begin(); // RFID
  lecteurRFID1.init(); // Initialisation lecteur RFID 1
  Serial.println(F("Scan pour voir les données"));
  Wire.begin(SLAVE_RFID_ADDR);

  // Function to run when data requested by master
  Wire.onRequest(requestEvent);

}

void requestEvent() {
  Wire.write(rfidState);
}

void loop() {
  // put your main code here, to run repeatedly:

  /*---------------------------------------------------------*/
  /*---------------------- PARTIE RFID ----------------------*/
  /*---------------------------------------------------------*/
  // LECTURE RFID
  if (lecteurRFID1.isCard()) {
    if (lecteurRFID1.readCardSerial()) {
      if (DEBUG) {
        Serial.print("UID RFID 1 : ");
      }
      for (int i=0; i <= 4; i++) {
        UID1[i]=lecteurRFID1.serNum[i];
        if (DEBUG) {
          Serial.print(UID1[i],DEC);
          Serial.print(".");
        }
      }
      if (DEBUG) {
        Serial.println("");
      }
    }
   if ((UID1[0] == MASTERKEY[0]
     && UID1[1] == MASTERKEY[1]
     && UID1[2] == MASTERKEY[2]
     && UID1[3] == MASTERKEY[3]
     && UID1[4] == MASTERKEY[4]))
    {
      RFID1Checked = true;
      rfidState = 1;
      if (DEBUG) {
        Serial.println("OK !!");
        Serial.println("RFID 1 Checked !");
      }
    }
    else
    {
      if (DEBUG) {
        Serial.println("PAS BON !!!!");
      }
    }          
    lecteurRFID1.halt();
  }
  else {
    RFID1Checked = false;
    //Serial.println("RFID 1 No checked !");
  }
  delay(10);

}
