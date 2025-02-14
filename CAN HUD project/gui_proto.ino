#include "SPI.h"
#include "TFT_eSPI.h"
#include <mcp2515.h>


TFT_eSPI tft = TFT_eSPI();

struct can_frame canMsg;
MCP2515 mcp2515(D4);


#define NEEDLESTYLE drawTriangleNeedle
#define REDLINE 6500
#define GAUGEMAX 8000
#define GAUGERADIUS 100
#define GAUGETHICKNESS 10
#define GAUGERESOLUTION 8000

#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF 
#define WHITE    0xFFFF
#define ORANGE 0xFC00
#define GREY 0x4208


int displayedRpm = 0;
int rpm = 0;
int displayedSpeed = 0;
int speed = 0;
int displayedTemp = 0;
int temp = 0;

void setup() { 
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setListenOnlyMode();
  
  tft.begin();
  
  tft.setRotation(7);
  tft.setTextSize(8);
  tft.setTextColor(WHITE);
  
  tft.fillScreen(RED);
  delay(100);
  tft.fillScreen(GREEN);
  delay(100);
  tft.fillScreen(BLUE);
  delay(100);
  tft.fillScreen(BLACK);
  delay(1000);
  
  drawGauge(WHITE);
  printSpeed(0, WHITE);
  printTemp(0, WHITE);
  NEEDLESTYLE(0, RED);
  delay(100);
  for(int i = -400; i <= 400; i++)
  {
    rpm = 8000 - 20*abs(i);
    temp = map(rpm, 0,8000, 0,150);
    speed = map(rpm, 0,8000, 0,220);
    updateRpm();
    updateTemp();
    updateSpeed();
      delay(1);
  }//*/
  
}
bool d = false;

void loop(void) {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    if(!d)
    {
      tft.drawCircle(0,0,20,RED);
      d = true;
    }
    
    if(canMsg.can_id == 0x124)
    {
      rpm = (((uint16_t)canMsg.data[1]<<8) + canMsg.data[2])/4;
    }
    else if(canMsg.can_id == 0x338)
    {
      temp = canMsg.data[1];
    }
    else if(canMsg.can_id == 0x310)
    {
      speed = (((uint16_t)canMsg.data[1]<<8) + (uint16_t)canMsg.data[2])/115;
    }
  }
  
  updateSpeed();
  updateRpm();
  updateTemp();
}

void printSpeed(int s, uint16_t color)
{
  tft.setTextSize(8);
  tft.setTextColor(color);
  if(color != BLACK && s >= 135) tft.setTextColor(ORANGE);
  if(s<10)
  {
    tft.setCursor(145, 92);
    tft.print(s);
  }
  else if(s<100)
  {
    tft.setCursor(121, 92);
    tft.print(s);
  }
  else
  {
    tft.setCursor(97, 92);
    tft.print(s);
  }
}

void updateSpeed()
{
 if(speed != displayedSpeed)
 {
    printSpeed(displayedSpeed, BLACK);
    displayedSpeed = speed;
    printSpeed(displayedSpeed, WHITE);
 }
}

void drawGauge(uint16_t color)
{
  for(int i = 0; i < GAUGERESOLUTION; i++)
  {
    int thModifier = 0;
    uint16_t drawColor = color;
    if(i/(float)GAUGERESOLUTION >= (float)REDLINE/(float)GAUGEMAX)
    {
      drawColor = RED;
    }
    if(i%(GAUGEMAX/GAUGERESOLUTION*1000) < (GAUGERESOLUTION/80) && i > (GAUGEMAX/GAUGERESOLUTION)) 
    {
      drawColor = GREY;
      thModifier = 3;
    }
    float j = PI / GAUGERESOLUTION * (float)i * 3.0 / 2.0 + 3.0* PI / 4.0;
    if( i%(GAUGERESOLUTION/40) < (GAUGERESOLUTION/80))
    {
      tft.drawLine(cos(j)*(GAUGERADIUS-thModifier)+160, sin(j)*(GAUGERADIUS-thModifier)+120, 
      cos(j)*(GAUGERADIUS+GAUGETHICKNESS)+160, sin(j)*(GAUGERADIUS+GAUGETHICKNESS)+120, drawColor);  
    }
  }
}

void drawSquareNeedle(int value, uint16_t color)
{
  int baseWidth = 100; // in "rpm"
  int height = 25;
  float i = PI / GAUGERESOLUTION * (float)value * 3.0 / 2.0 + 3.0* PI / 4.0;               //  j--------j
  float j = PI / GAUGERESOLUTION * (float)(value + baseWidth) * 3.0 / 2.0 + 3.0* PI / 4.0; //  i--------i
  tft.fillTriangle(cos(i)*(GAUGERADIUS - 7)+160, sin(i)*(GAUGERADIUS - 7)+120, 
  cos(i)*(GAUGERADIUS - height - 7) + 160, sin(i)*(GAUGERADIUS - height - 7) + 120,
  cos(j)*(GAUGERADIUS - height - 7) + 160, sin(j)*(GAUGERADIUS - height - 7) + 120,color);

  tft.fillTriangle(cos(j)*(GAUGERADIUS - height - 7)+160, sin(j)*(GAUGERADIUS - height - 7)+120, 
  cos(i)*(GAUGERADIUS - 7) + 160, sin(i)*(GAUGERADIUS - 7) + 120,
  cos(j)*(GAUGERADIUS - 7) + 160, sin(j)*(GAUGERADIUS - 7) + 120,color);
}

void drawTriangleNeedle(int value, uint16_t color)
{
  int baseWidth = 250; // in "rpm"
  int height = 25;
  float k = PI / GAUGERESOLUTION * (float)value * 3.0 / 2.0 + 3.0* PI / 4.0;
  float i = PI / GAUGERESOLUTION * (float)(value - baseWidth/2) * 3.0 / 2.0 + 3.0* PI / 4.0; //  i---------i
  float j = PI / GAUGERESOLUTION * (float)(value + baseWidth/2) * 3.0 / 2.0 + 3.0* PI / 4.0; //  j---------j
  tft.fillTriangle(cos(k)*(GAUGERADIUS - 5)+160, sin(k)*(GAUGERADIUS - 5)+120, 
  cos(i)*(GAUGERADIUS - height - 5) + 160, sin(i)*(GAUGERADIUS - height - 5) + 120,
  cos(j)*(GAUGERADIUS - height - 5) + 160, sin(j)*(GAUGERADIUS - height - 5) + 120,color);
}

void updateRpm()
{
  if(rpm != displayedRpm)
  {
    NEEDLESTYLE(displayedRpm, BLACK);
    displayedRpm = rpm;
    NEEDLESTYLE(displayedRpm, RED);
  }
}

void printTemp(int value, uint16_t color)
{
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(color);
  if(color != BLACK && value > 110) 
  {
    tft.setTextColor(RED);
  }
  else if(color != BLACK && value < 75)
  {
    tft.setTextColor(BLUE);
  }
  else if (color != BLACK)
  {
    tft.setTextColor(GREEN);
  }
  tft.setTextSize(4);
  tft.drawString(String(value) + "C", 160, 210);
}

void updateTemp()
{
  if(temp != displayedTemp)
  {
    printTemp(displayedTemp, BLACK);
    displayedTemp = temp;
    printTemp(displayedTemp, WHITE);
  }
}
