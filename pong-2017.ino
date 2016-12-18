#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;

class Player {
  public:
    Player();
    byte x, y;
    byte xMin, yMin;
    byte padSize;
    char name[4];
};

Player::Player() {
  padSize = 7;
  name[4] = "YQN";
}

extern const byte font3x3[];

Player p1, p2;

void drawBackground() {
  gb.display.clear();

  gb.display.setColor(BLACK);
  gb.display.drawFastHLine(0, 0, LCDWIDTH);
  gb.display.drawFastHLine(0, 47, LCDWIDTH);

  gb.display.setColor(GRAY);
  gb.display.fillRect(41, 1, 2, 46);
  
  gb.display.cursorX = 29;
  gb.display.cursorY = 2;
  gb.display.print("YQN");
  gb.display.cursorX = 44;
  gb.display.print("YQN");
  
  gb.display.fillRect(1, 2, 27, 3);
  gb.display.drawPixel(0, 3);
  gb.display.fillRect(56, 2, 27, 3);
  gb.display.drawPixel(83, 3);
}

void setup() {
  Serial.begin(9600);
  gb.begin();
  gb.display.setFont(font3x3);
  gb.titleScreen(F("PONG 2017"));
  //gb.pickRandomSeed();
  gb.battery.show = false;
}

void loop() {
  if (gb.update()) {
    drawBackground();

    if (gb.buttons.pressed(BTN_C)) gb.titleScreen(F("PONG 2017"));
  }
}
