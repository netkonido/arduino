#include <Adafruit_NeoPixel.h>
#include <enums.ino>

#define MAXPERCENT 255
#define LEDCOUNT 240
#define CONTROLPIN 3
#define RELAY 4
#define BUTTON 5

#define RGBSPEED 25
#define SUNSETSPEED 5
#define LAVASPEED 20
#define LAVAWIDTH 30
#define FADESPEED 15
#define SOLIDFADEMODULATION 25 //     HANG                |...
#define FADECHANGE 150 //  Color2     /----\  MULTxHANG   /|...
#define FADEHANG 350    // Color1 ---/      \------------/ |...
#define FADEMULT 1.5 //               ^CHANGE^              |...


typedef uint32_t Color;
Adafruit_NeoPixel strip(LEDCOUNT, CONTROLPIN, NEO_RGB + NEO_KHZ800);


enum Mode mode = FADE;
Color stripColors[LEDCOUNT];

int colorAngle = 0;

Color solidColor = WHITE;

Color fadeColor1 = 0x0056ff00;
Color fadeColor2 = 0x0034ff00;
//Color fadeColor1 = RED;
//Color fadeColor2 = PURPLE;
//Color fadeColor1 = BLUE;
//Color fadeColor2 = 0x009100ff;
//Color fadeColor2 = 0x00ff0000;
//Color fadeColor1 = 0x000000ff;

Color lavaBase = ORANGE;
Color lavaTop = 0x0099ff00;
Color lavaBottom = 0x0044ff00;
int lavaModulation[LEDCOUNT];

Color glacierBase = CYAN;
Color glacierTop = WHITE;
Color glacierBottom = BLUE;

int stepCount = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(CONTROLPIN, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUTTON, INPUT);
  strip.begin();
  strip.clear();
  for (int i = 0; i < LEDCOUNT; i++)
  {
    if (mode == LAVA)
    {
      lavaModulation[i] = 0;
      strip.setPixelColor(i, lavaBase);
      stripColors[i] = lavaBase;
    }
    else if (mode == GLACIER)
    {
      strip.setPixelColor(i, glacierBase);
      stripColors[i] = glacierBase;
    }
    strip.show();
    //delay(3);
    // strip.ColorHSV(map(i, 0,LEDCOUNT, 0, 65536),255,MAXPERCENT);
  }
  delay(1000);
  randomSeed(analogRead(0));

  strip.setBrightness(30);
}


void loop() {
  if (mode == RGB)
  {
    rotate();
    colorAngle = (colorAngle + 1) % 360;
    stripColors[0] = strip.ColorHSV( map(colorAngle, 0, 360, 0, 65536), 255, MAXPERCENT);
    update();
    strip.show();
    delay(1000 / RGBSPEED);
  }
  else if (mode == SOLIDCOLOR)
  {
    strip.fill(solidColor);
    strip.show();
  }
  else if (mode == SOLIDFADE)
  {
    rotate();
    colorAngle = (colorAngle + 7) % 720;
    float modulation = (float)((sin((float)colorAngle / 360.0 * PI) + 1) * SOLIDFADEMODULATION) / 100.0;
    stripColors[0] = strip.gamma32(combine(getR(solidColor) * modulation, getG(solidColor) * modulation, getB(solidColor) * modulation));
    update();
    strip.show();
    delay(1000 / FADESPEED);
  }
  else if (mode == FADE)
  {
    rotate();
    if (stepCount <= FADECHANGE)
    { 
      //fade from color 1 to color2 in FADECHANGE steps
      // component = color1 + (((color2-color1)*step)/steps)
      int currentStep = stepCount;
      int redComponent =   (int)getR(fadeColor1) + (int)((float)(((float)getR(fadeColor2)-(float)getR(fadeColor1))* (float)currentStep) / (float)FADECHANGE);
      int greenComponent = (int)getG(fadeColor1) + (int)((float)(((float)getG(fadeColor2)-(float)getG(fadeColor1))* (float)currentStep) / (float)FADECHANGE);
      int blueComponent =  (int)getB(fadeColor1) + (int)((float)(((float)getB(fadeColor2)-(float)getB(fadeColor1))* (float)currentStep) / (float)FADECHANGE);
      stripColors[0] = strip.gamma32(strip.Color(redComponent, greenComponent, blueComponent));
    }
    else if (stepCount <= FADECHANGE + FADEHANG)
    {
      //display color2
      stripColors[0] = strip.gamma32(strip.Color(getR(fadeColor2),getG(fadeColor2), getB(fadeColor2)));
    }
    else if (stepCount <= FADECHANGE + FADEHANG + FADECHANGE)
    {
      //fade from color2 to color1
      // component = color2 + (((color1-color2)*step)/steps)
      int currentStep = stepCount - FADECHANGE - FADEHANG;
      int redComponent =   (int)getR(fadeColor2) + (int)((float)(((float)getR(fadeColor1)-(float)getR(fadeColor2))* (float)currentStep) / (float)FADECHANGE);
      int greenComponent = (int)getG(fadeColor2) + (int)((float)(((float)getG(fadeColor1)-(float)getG(fadeColor2))* (float)currentStep) / (float)FADECHANGE);
      int blueComponent =  (int)getB(fadeColor2) + (int)((float)(((float)getB(fadeColor1)-(float)getB(fadeColor2))* (float)currentStep) / (float)FADECHANGE);
      stripColors[0] = strip.gamma32(strip.Color(redComponent, greenComponent, blueComponent));
    }
    else if (stepCount <= FADECHANGE + FADEHANG + FADECHANGE + (FADEMULT * FADEHANG))
    {
      //display color1
      stripColors[0] = strip.gamma32(strip.Color(getR(fadeColor1),getG(fadeColor1), getB(fadeColor1)));
    }
    else
    {
      stepCount = 0;
    }
    update();
    strip.show();
    delay(1000 / FADESPEED);
    stepCount++;
  }
  else if (mode == LAVA)
  {
    colorAngle = (colorAngle + 1) % 20;
    //Serial.println(colorAngle);
    if (colorAngle == 0)
    {
      lavaRandom();
    }
    lavaIterate(lavaBottom, lavaBase, lavaTop);
    update();
    strip.show();
  }
  else if(mode == TEST)
  {
    strip.fill(RED);
    strip.show();
    delay(500);
    strip.fill(GREEN);
    strip.show();
    delay(500);
    strip.fill(BLUE);
    strip.show();
    delay(500);
  }

  /*Serial.println("------------------------------");
    Serial.print("new color: {");
    Serial.print(stripColors[0].R);
    Serial.print(", ");
    Serial.print(stripColors[0].G);
    Serial.print(", ");
    Serial.print(stripColors[0].B);
    Serial.println("}");
  */
}

void rotate()
{
  for (int i = LEDCOUNT - 1; i > 0; i--)
  {
    stripColors[i] = stripColors[i - 1];
  }
}

void update()
{
  for (int i = 0; i < LEDCOUNT; i++)
  {
    strip.setPixelColor(i, stripColors[i]);
  }
}

uint8_t getR(Color c)
{
  return (c >> 16);
}

uint8_t getG(Color c)
{
  return (c >> 8);
}

uint8_t getB(Color c)
{
  return c;
}
Color combine(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b);
}


