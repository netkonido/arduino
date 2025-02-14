#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(4);


void setup() {
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setListenOnlyMode();
  while(!Serial){delay(10);}
}
uint16_t rpm = 0;
uint16_t vspd = 0;
uint16_t ctmp = 0;
void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    if(canMsg.can_id == 0x124)
    {
      rpm = (((uint16_t)canMsg.data[1]<<8) + canMsg.data[2])/4;
    }
    else if(canMsg.can_id == 0x338)
    {
      ctmp = canMsg.data[1];
    }
    else if(canMsg.can_id == 0x310)
    {
      vspd = (((uint16_t)canMsg.data[1]<<8) + (uint16_t)canMsg.data[2])/115;
    }
    Serial.print("Engine_rpm:");
      Serial.print(rpm);
      Serial.print(",Coolant_temp:");
      Serial.print(ctmp);
      Serial.print(",Vehicle_speed:");
      Serial.print(vspd);
      Serial.println();
  }
}
