#include "Arduino.h"
#include "LoRa_E32.h"
#include <SoftwareSerial.h>

// Khởi tạo mô-đun LoRa với TX trên pin 6 và RX trên pin 7 của Arduino
LoRa_E32 e32ttl100(6, 7); 

void printParameters(struct Configuration configuration);
void printModuleInformation(struct ModuleInformation moduleInformation);

void setup() {
    Serial.begin(9600);
    delay(500);

    // Khởi động tất cả các chân và giao tiếp UART
    e32ttl100.begin();

    ResponseStructContainer c;
    c = e32ttl100.getConfiguration();
    
    // Lấy cấu hình hiện tại từ mô-đun
    Configuration configuration = *(Configuration*) c.data;
    Serial.println(c.status.getResponseDescription());
    Serial.println(c.status.code);

    // In các tham số cấu hình hiện tại
    printParameters(configuration);

    // Thiết lập địa chỉ và kênh
    configuration.ADDL = 0x1;    // Địa chỉ thấp là 1, nếu là Lora2 thì là 0x2
    configuration.ADDH = 0x0;    // Địa chỉ cao là 0
    configuration.CHAN = 0x19;   // Kênh 25 (433MHz + 25 * 1MHz = 458MHz)

    // Cấu hình các tùy chọn khác
    configuration.OPTION.fec = FEC_1_ON;
    configuration.OPTION.transmissionPower = POWER_10; // Giảm công suất truyền để tiết kiệm năng lượng
    // configuration.OPTION.fixedTransmission = FT_TRANSPARENT_TRANSMISSION;
    // configuration.OPTION.ioDriveMode = IO_D_MODE_PUSH_PULLS_PULL_UPS;
    // configuration.OPTION.wirelessWakeupTime = WAKE_UP_1250;

    // Thiết lập tốc độ truyền dữ liệu và các thông số UART
    configuration.SPED.airDataRate = AIR_DATA_RATE_010_24; // Giảm tốc độ truyền dữ liệu để tiết kiệm năng lượng
    configuration.SPED.uartBaudRate = UART_BPS_9600; // Giảm tốc độ UART để tiết kiệm năng lượng
    configuration.SPED.uartParity = MODE_00_8N1;

    // Gửi cấu hình mới đến mô-đun và không giữ cấu hình sau khi tắt nguồn
    ResponseStatus rs = e32ttl100.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
    Serial.println(rs.getResponseDescription());
    Serial.println(rs.code);

    // In các tham số cấu hình mới
    printParameters(configuration);
    c.close();
}

void loop() {
    // Hàm loop trống vì ta chỉ cần cấu hình mô-đun một lần trong setup()
}

void printParameters(struct Configuration configuration) {
    Serial.println("----------------------------------------");

    Serial.print(F("HEAD : "));  
    Serial.print(configuration.HEAD, BIN); Serial.print(" "); 
    Serial.print(configuration.HEAD, DEC); Serial.print(" "); 
    Serial.println(configuration.HEAD, HEX);
    Serial.println(F(" "));
    Serial.print(F("AddH : "));  Serial.println(configuration.ADDH, BIN);
    Serial.print(F("AddL : "));  Serial.println(configuration.ADDL, BIN);
    Serial.print(F("Chan : "));  Serial.print(configuration.CHAN, DEC); 
    Serial.print(" -> "); 
    Serial.println(configuration.getChannelDescription());
    Serial.println(F(" "));
    Serial.print(F("SpeedParityBit     : "));  
    Serial.print(configuration.SPED.uartParity, BIN); 
    Serial.print(" -> "); 
    Serial.println(configuration.SPED.getUARTParityDescription());
    Serial.print(F("SpeedUARTDatte  : "));  
    Serial.print(configuration.SPED.uartBaudRate, BIN); 
    Serial.print(" -> "); 
    Serial.println(configuration.SPED.getUARTBaudRate());
    Serial.print(F("SpeedAirDataRate   : "));  
    Serial.print(configuration.SPED.airDataRate, BIN); 
    Serial.print(" -> "); 
    Serial.println(configuration.SPED.getAirDataRate());

    Serial.print(F("OptionTrans        : "));  
    Serial.print(configuration.OPTION.fixedTransmission, BIN); 
    Serial.print(" -> "); 
    Serial.println(configuration.OPTION.getFixedTransmissionDescription());
    Serial.print(F("OptionPullup       : "));  
    Serial.print(configuration.OPTION.ioDriveMode, BIN); 
    Serial.print(" -> "); 
    Serial.println(configuration.OPTION.getIODroveModeDescription());
    Serial.print(F("OptionWakeup       : "));  
    Serial.print(configuration.OPTION.wirelessWakeupTime, BIN); 
    Serial.print(" -> "); 
    Serial.println(configuration.OPTION.getWirelessWakeUPTimeDescription());
    Serial.print(F("OptionFEC          : "));  
    Serial.print(configuration.OPTION.fec, BIN); 
    Serial.print(" -> "); 
    Serial.println(configuration.OPTION.getFECDescription());
    Serial.print(F("OptionPower        : "));  
    Serial.print(configuration.OPTION.transmissionPower, BIN); 
    Serial.print(" -> "); 
    Serial.println(configuration.OPTION.getTransmissionPowerDescription());

    Serial.println("----------------------------------------");
}
