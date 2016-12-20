#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;

#define EXPANDPAD 0
#define SHRINKPAD 1
#define EXTRAPAD 2
#define FREEZE 3
#define INVISIBALL 4

const byte trickDuration[5] = { 60, 60, 60, 20, 30 };

const char trickName[5][11] PROGMEM =
{
  "Expand pad",
  "Shrink pad",
  "Extra pad",
  "Freeze",
  "Invisiball"
};

class Player {
  private:
    byte trickFC;
    
  public:
    Player();
    byte id;
    byte x, y;
    byte padWidth, padHeight;
    byte roundsWon;
    float xSpeed, ySpeed;
    byte points;
    int roundsBarDir;
    byte roundsBarX;
    bool tricksMenuOn;
    byte trick[5];
    bool trickOn;
    byte tricksCursor;

    void initPlayer(byte p);
    byte padTop(); // returns the top pixel of the pad
    void drawPad();
    void moveUp();
    void moveDown();
    void updateTricksMenu();
    void updateTrick();
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
  tricksCursor = 2;
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

void Player::updateTricksMenu() {
  if (tricksMenuOn) {
    gb.display.cursorX = 10 + tricksCursor*4;
    gb.display.cursorY = 39;
    gb.display.println(char(25));
    gb.display.cursorX = 10;
    gb.display.print("01234");
  }
}

void Player::updateTrick() {
  if (trickOn) {
    if (trickFC == trickDuration[tricksCursor]) {
      padHeight = 7;
      trickOn = false;
      trickFC = 0;
      return;
    }
    switch (tricksCursor) {
      case EXPANDPAD:
        padHeight = 9;
        break;
    }
    trickFC++;
  }
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
  for (byte i = 0; i < 2; i++) {
    player[i].initPlayer(i);
    for (byte j = 0; j < 5; j++) {
      player[i].trick[j] = j;
    }
  }

}

void loop() {
  if (gb.update()) {
    drawBackground();

    for (byte i = 0; i < 2; i++) player[i].updateTrick();

    ball.move();
    ball.padCollide();
    ball.draw();

    player[0].updateTricksMenu();

    for (byte i = 0; i < 2; i++) { player[i].drawPad(); }

    // move p2
    if (ball.y > player[1].y) player[1].moveDown();
    if (ball.y < player[1].y) player[1].moveUp();

    if (gb.buttons.repeat(BTN_UP, 1)) player[0].moveUp();
    if (gb.buttons.repeat(BTN_DOWN, 1)) player[0].moveDown();

    if (gb.buttons.pressed(BTN_LEFT) && player[0].tricksMenuOn && player[0].tricksCursor > 0)
      player[0].tricksCursor--;
    if (gb.buttons.pressed(BTN_RIGHT) && player[0].tricksMenuOn && player[0].tricksCursor < 4)
      player[0].tricksCursor++;

    if (gb.buttons.pressed(BTN_B) && !player[0].trickOn) {
      player[0].tricksMenuOn = -player[0].tricksMenuOn+1;
    }

    if (gb.buttons.pressed(BTN_A) && player[0].tricksMenuOn) {
      gb.sound.playOK();
      player[0].tricksMenuOn = false;
      player[0].trickOn = true;      
    }
    
    if (gb.buttons.pressed(BTN_C)) {
      gb.titleScreen(F("PONG 2017"));
      gb.battery.show = false;
    }
  }
}
