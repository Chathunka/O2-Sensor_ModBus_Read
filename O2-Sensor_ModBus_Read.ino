#include <esp32ModbusRTU.h>
byte address=2;
esp32ModbusRTU modbus(&Serial2, 4);

void Init_modbus(){
  Serial2.begin(4800);
  Serial.println("Init MODBUS");
  modbus.onData([](uint8_t serverAddress, esp32Modbus::FunctionCode fc, uint16_t address, uint8_t* data, size_t length) 
    {
      digitalWrite(2,HIGH);
      Serial.print("\n");
      uint16_t combinedValue = (data[0] << 8) | data[1];
      long decimalValue = strtol(String(combinedValue, HEX).c_str(), NULL, 16);
      float floatValue = decimalValue / 10.0;
      Serial.print("O2 vol : ");
      Serial.print(floatValue);
      Serial.print("%");
    }
  );
  modbus.onError([](esp32Modbus::Error error) {
     Serial.printf("error: 0x%02x\n\n", static_cast<uint8_t>(error));
  });
  modbus.begin();
}

void setup() {
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(115200);
  Serial.print("GATEWAY init");
  delay(300);
  Init_modbus();
}


void loop(){ 
  modbus.readHoldingRegisters(address,0x00,1); // serverId, address + length
  delay(1000);
  digitalWrite(2,LOW);
}


