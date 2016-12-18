#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;

class Player {
  public:
    Player(bool);
    bool id;
    byte x, y;
    byte xMin, yMin;
    byte padSize;
    byte roundsWon;
    byte xSpeed, ySpeed;
    
    void drawPad();
    void moveUp();
    void moveDown();
};

Player::Player(bool p) {
  id = p;
  if (p == 0) x = 0;
  else x = 82;
  y = 23;
  padSize = 7;
  roundsWon = 5;
  ySpeed = 2;
}

void Player::drawPad() {
  byte top;
  top = y - floor(padSize/2);

  gb.display.setColor(BLACK);
  gb.display.fillRect(x, top, 2, padSize);
}

void Player::moveUp() {
  if (y > 2) y -= ySpeed;
}
void Player::moveDown() {
  if (y < LCDHEIGHT-2) y += ySpeed;
}

class Ball {
  public:
    Ball();
    byte x, y;
    int xSpeed, ySpeed;
    void draw();
    void move();
};

Ball::Ball() {
  x = 41;
  y = 23;
  xSpeed = 1;
  ySpeed = 1;
}

void Ball::draw() {
  gb.display.setColor(BLACK);
  gb.display.fillRect(x, y, 2, 2);
}

void Ball::move() {
  if (xSpeed > 0) {
    if (x < LCDWIDTH-2) x += xSpeed;
    else xSpeed = -xSpeed;
  }
  if (xSpeed < 0) {
    if (x > 0) x += xSpeed;
    else xSpeed = -xSpeed;
  }
  if (ySpeed > 0) {
    if (y < LCDHEIGHT-2) y += ySpeed;
    else ySpeed = -ySpeed;
  }
  if (ySpeed < 0) {
    if (y > 1) y += ySpeed;
    else ySpeed = -ySpeed;
  }
}

extern const byte font3x3[];

Player p1(0);
Player p2(1);
Ball ball;

void drawBackground() {
  gb.display.clear();

  // top & bottom borders
  gb.display.setColor(BLACK);
  gb.display.drawFastHLine(0, 0, LCDWIDTH);
  gb.display.drawFastHLine(0, 47, LCDWIDTH);

  // net
  gb.display.setColor(GRAY);
  gb.display.fillRect(41, 1, 2, 46);

  // names
  gb.display.cursorX = 29;
  gb.display.cursorY = 2;
  gb.display.print("YQN");
  gb.display.cursorX = 44;
  gb.display.print("RLF");

  // life gauges
  gb.display.fillRect(1, 2, 27, 3);
  gb.display.drawPixel(0, 3);
  gb.display.fillRect(56, 2, 27, 3);
  gb.display.drawPixel(83, 3);

  // rounds won
  for (byte i = 0; i < p1.roundsWon; i++) {
    byte x = 36 - i*4;
    gb.display.drawFastHLine(x, 6, 3);
  }
  for (byte i = 0; i < p2.roundsWon; i++) {
    byte x = 44 + i*4;
    gb.display.drawFastHLine(x, 6, 3);
  }
}

void setup() {
  Serial.begin(9600);
  gb.begin();
  gb.display.setFont(font3x3);
  gb.titleScreen(F("PONG 2017"));
  gb.pickRandomSeed();
  gb.battery.show = false;
}

void loop() {
  if (gb.update()) {
    drawBackground();

    p1.drawPad();
    p2.drawPad();

    ball.move();
    ball.draw();

    if (gb.buttons.repeat(BTN_UP, 1)) p1.moveUp();
    if (gb.buttons.repeat(BTN_DOWN, 1)) p1.moveDown();
    if (gb.buttons.pressed(BTN_C)) gb.titleScreen(F("PONG 2017"));
  }
}
