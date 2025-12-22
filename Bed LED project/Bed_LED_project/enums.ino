typedef uint32_t Color;

enum Mode
{
  TEST,
  RGB,//rgb spectrum lights
  SOLIDCOLOR, // single color on all leds
  FADE,  //fade from one color to the next with traveling lights
  SOLIDFADE, // solid color with travelling light/dark patches
  SUNSET, // slow fade between bright yellow to dark orange and back
  LAVA, // yellow-orange-red patterns fading in-out
  GLACIER, //same as lava except blue-cyan-white
  FOREST //
  //RUNNINGLIGHT // small bit of leds going front-back bounce style
};

enum basicColors : Color
{
  RED = 0x0000ff00,
  GREEN = 0x00ff0000,
  BLUE = 0x000000ff,
  CYAN = 0x00ff00ff,
  MAGENTA = 0x0000ffff,
  YELLOW = 0x00ffff00,
  ORANGE = 0x006fff00,
  PURPLE = 0x00007fff,
  PINK = 0x005fffff,

  WHITE = 0x00ffffff,
  BLACK = 0x00000000
};