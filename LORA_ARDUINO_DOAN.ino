#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// LoRa and GPS serial connections
SoftwareSerial mySerial(6, 7); // LoRa TX, RX
SoftwareSerial gpsSerial(2, 3); // GPS TX, RX

const int light = A0;
const int light1 = A1;
#define M0 9
#define M1 8
#define LED_PIN 13

TinyGPSPlus gps; // Create a TinyGPS++ object

unsigned long previousMillis = 0;
const long interval = 6000;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  gpsSerial.begin(9600);

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(M0, LOW);      
  digitalWrite(M1, LOW);
}

void loop() {

  if (Serial.available() > 0) {
    String input = Serial.readString();
    mySerial.println(input);
  }


  if (mySerial.available() > 1) {
    String input = mySerial.readString();
    Serial.print(input);
  }

  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  unsigned long currentMillis = millis(); 


  if (currentMillis - previousMillis >= interval) {
    if (gps.location.isUpdated()) {
      float latitude = gps.location.lat();
      float longitude = gps.location.lng();


      String gpsString = String(latitude, 6) + "," + String(longitude, 6);
      Serial.println("Sending GPS data via LoRa: " + gpsString);
      mySerial.println(gpsString);

      previousMillis = currentMillis; 
    }
  }

  delay(100); 
}
