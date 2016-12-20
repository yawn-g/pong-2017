#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;

class Player {
  private:
    
  public:
    Player();
    byte id;
    byte x, y;
    byte padWidth, padHeight;
    byte roundsWon;
    byte xSpeed, ySpeed;
    byte points;
    int roundsBarDir;
    byte roundsBarX;

    void initPlayer(byte p);
    byte padTop(); // return the top pixel of the pad
    void drawPad();
    void moveUp();
    void moveDown();
};

Player::Player() {
  y = 23;
  padHeight = 7;
  padWidth = 2;
  roundsWon = 5;
  ySpeed = 2;
}

void Player::initPlayer(byte p) {
  id = p;
  if (id == 0) {
    x = 0;
    roundsBarX = 36;
    roundsBarDir = -1;
  }
  else {
    x = 82;
    roundsBarX = 44;
    roundsBarDir = 1;
  }
}

byte Player::padTop() {
  return y - floor(padHeight/2);
}
void Player::drawPad() {
  gb.display.setColor(BLACK);
  gb.display.fillRect(x, padTop(), padWidth, padHeight);
  // debug
  /*
  gb.display.cursorY = padTop() +1;
  gb.display.cursorX = 4;
  gb.display.print(id);
  gb.display.print(": x=");
  gb.display.print(x);*/
}

void Player::moveUp() {
  if (padTop() > 1) y -= ySpeed;
}
void Player::moveDown() {
  if (padTop() + padHeight < LCDHEIGHT-2) y += ySpeed;
}

Player player[2];

class Ball {
  private:
    byte width;
    
  public:
    Ball();
    byte x, y;
    float xSpeed, ySpeed;
    void draw();
    void move();
    void padCollide();
};

Ball::Ball() {
  x = 41;
  y = 23;
  xSpeed = 1;
  ySpeed = 1;
  width = 2;
}

void Ball::draw() {
  gb.display.setColor(BLACK);
  gb.display.fillRect(x, y, width, width);
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

void Ball::padCollide() {
  for (byte p = 0; p < 2; p++) {
    if (gb.collideRectRect(x, y, width, width, player[p].x, player[p].padTop(), player[p].padWidth, player[p].padHeight)) {
      gb.sound.playTick();
      xSpeed = -xSpeed;
      //move();
    }
  }
}

extern const byte font3x3[];

Ball ball;

void drawBackground();

void setup() {
  Serial.begin(9600);
  gb.begin();
  gb.display.setFont(font3x3);
  gb.titleScreen(F("PONG 2017"));
  gb.pickRandomSeed();
  gb.battery.show = false;
  for (byte i = 0; i < 2; i++) player[i].initPlayer(i);
}

void loop() {
  if (gb.update()) {
    drawBackground();

    ball.move();
    ball.padCollide();
    ball.draw();

    for (byte i = 0; i < 2; i++) { player[i].drawPad(); }

    // move p2
    if (ball.y > player[1].y) player[1].moveDown();
    if (ball.y < player[1].y) player[1].moveUp();

    if (gb.buttons.repeat(BTN_UP, 1)) player[0].moveUp();
    if (gb.buttons.repeat(BTN_DOWN, 1)) player[0].moveDown();
    if (gb.buttons.pressed(BTN_C)) {
      gb.titleScreen(F("PONG 2017"));
      gb.battery.show = false;
    }
  }
}
