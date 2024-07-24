#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Firebase.h>

const long interval = 3000;
unsigned long previousMillis = 0;

#define M0 19
#define M1 21
#define WIFI_SSID "Redmi 10C"
#define WIFI_PASSWORD "16062002"
#define REFERENCE_URL "https://mqtt-b161c-default-rtdb.firebaseio.com"  

Firebase firebase(REFERENCE_URL);

void setup() {
  Serial2.begin(9600);   
  Serial.begin(9600);

  // Kết nối WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Đang kết nối Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Đã kết nối với IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  pinMode(M0, OUTPUT);        
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW);       
  digitalWrite(M1, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  if (Serial2.available() > 0 && currentMillis - previousMillis >= interval) {
    String input = Serial2.readStringUntil('\n');
    Serial.print("Dữ liệu thô nhận được: ");
    Serial.println(input);

    float latitude = 0.0;
    float longitude = 0.0;
    if (parseGPSData(input, latitude, longitude)) {
      String latString = String(latitude, 6); 
      String longString = String(longitude, 6);

      bool resultLat = firebase.setString("53/Lat", latString);
      bool resultLong = firebase.setString("53/Long", longString);

      if (resultLat && resultLong) {
        Serial.println("Dữ liệu đã được gửi lên Firebase thành công.");
      } else {
        Serial.println("Gửi dữ liệu lên Firebase thất bại.");
      }
      previousMillis = currentMillis;
    } else {
      Serial.println("Dữ liệu không đúng định dạng GPS.");
    }
  }

  delay(1000);  
}

bool parseGPSData(String data, float &latitude, float &longitude) {
  int commaIndex = data.indexOf(',');
  if (commaIndex == -1) {
    Serial.println("Chuỗi nhận được không có dấu phẩy.");
    return false;
  }

  String latString = data.substring(0, commaIndex);
  String longString = data.substring(commaIndex + 1);

  latitude = latString.toFloat();
  longitude = longString.toFloat();

  Serial.print("Kết quả phân tích - Vĩ độ: ");
  Serial.print(latitude, 6);
  Serial.print(", Kinh độ: ");
  Serial.println(longitude, 6);

  return true;
}
