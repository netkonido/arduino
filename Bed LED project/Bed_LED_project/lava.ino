void lavaIterate(Color bottom, Color base, Color top)
{
  for (int i = 0; i < LEDCOUNT; i++)
  {
    int lmi = lavaModulation[i];
    for (int j = 0; j < abs(lmi); j++)
    {
      Color* currentColor = &stripColors[i + j];
      if (lmi > 0)
      {
        int incrementR = (getR(top) - getR(base)) / (LAVAWIDTH / 2);
        int incrementG = (getG(top) - getG(base)) / (LAVAWIDTH / 2);
        int incrementB = (getB(top) - getB(base)) / (LAVAWIDTH / 2);
        if (abs(lmi) < LAVAWIDTH / 2)
        {
          //increment color towards top
          if (i + j < LEDCOUNT && i + j > 0)
          {
            stripColors[i + j] = combine(getR(stripColors[i + j]) + incrementR, getG(stripColors[i + j]) + incrementG, getB(stripColors[i + j]) + incrementB);
          }
          if (i - j < LEDCOUNT && i - j > 0)
          {
            stripColors[i + j] = combine(getR(stripColors[i + j]) + incrementR, getG(stripColors[i + j]) + incrementG, getB(stripColors[i + j]) + incrementB);
          }
        }
        else
        {
          //increment color towards base
          int x = ((LAVAWIDTH / 2) - (j % (LAVAWIDTH / 2)));
          if (i + x < LEDCOUNT && i + x > 0)
          {
            stripColors[i + x] = combine(getR(stripColors[i + x]) - incrementR, getG(stripColors[i + x]) - incrementG, getB(stripColors[i + x]) - incrementB);
          }
          if (i - x < LEDCOUNT && i - x > 0)
          {
            stripColors[i + x] = combine(getR(stripColors[i + x]) - incrementR, getG(stripColors[i + x]) - incrementG, getB(stripColors[i + x]) - incrementB);
          }
        }
      }
      else
      {
        int incrementR = (getR(bottom) - getR(base)) / (LAVAWIDTH / 2);
        int incrementG = (getG(bottom) - getG(base)) / (LAVAWIDTH / 2);
        int incrementB = (getB(bottom) - getB(base)) / (LAVAWIDTH / 2);

        if (abs(lmi) < LAVAWIDTH / 2)
        {
          //increment color towards bottom
          if (i + j < LEDCOUNT && i + j >= 0)
          {
            stripColors[i + j] = combine(getR(stripColors[i + j]) + incrementR, getG(stripColors[i + j]) + incrementG, getB(stripColors[i + j]) + incrementB);
          }
          if (i - j < LEDCOUNT && i - j >= 0)
          {
            stripColors[i + j] = combine(getR(stripColors[i + j]) + incrementR, getG(stripColors[i + j]) + incrementG, getB(stripColors[i + j]) + incrementB);
          }
        }
        else
        {
          //increment color towards base
          int x = ((LAVAWIDTH / 2) - (j % (LAVAWIDTH / 2)));
          if (i + x < LEDCOUNT && i + x >= 0)
          {
            stripColors[i + x] = combine(getR(stripColors[i + x]) - incrementR, getG(stripColors[i + x]) - incrementG, getB(stripColors[i + x]) - incrementB);
          }
          if (i - x < LEDCOUNT && i - x >= 0)
          {
            stripColors[i + x] = combine(getR(stripColors[i + x]) - incrementR, getG(stripColors[i + x]) - incrementG, getB(stripColors[i + x]) - incrementB);
          }
        }
      }
    }
    lavaModulation[i] += sgn(lavaModulation[i]);
    lavaModulation[i] %= LAVAWIDTH;
  }
}

void lavaRandom()
{
  int r = random(0, LEDCOUNT);
  int* p = &lavaModulation[r];
  int q = random(0, 2);
  if (q == 0) q = -1;
  for (int i = 0; i <= LAVAWIDTH / 4; i++)
  {
    if (r + i >= 0 && r + i < LEDCOUNT)
    {
      if (lavaModulation[r + i] != 0)
      {
        q = 0;
      }
    }
    if (r - i >= 0 && r - i < LEDCOUNT)
    {
      if (lavaModulation[r - i] != 0)
      {
        q = 0;
      }
    }
  }
  if (q != 0)
  {
    p = q;
  }
}

int sgn(int a)
{
  if (a > 0) return 1;
  if (a < 0) return -1;
  return 0;
}