#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
#include <Wire.h>

/*
 * CONFIG
 */

#define PAD_HEIGHT 7
#define HEALTH 27
#define TRICKS_PER_PLAYER 5
byte nbRounds = 5;
bool ballVisible = true;
bool hideHealth = false;

 
/*
 * TRICKS
 * number is ID and ASCII code
 */

#define INVISIBALL 42  // *
#define BORDERS 61     // =
#define EXTRA_PAD 67   // C
#define FREEZE 70      // F
#define SHRINK_PAD 72  // H
#define EXPAND_PAD 88  // X

byte tricks[128];

const byte trickDuration[128] = {
  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 0-9
  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 10-19
  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 20-29
  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 30-39
  10, 10, 25, 10, 10, 10, 10, 10, 10, 10, // 40-49
  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 50-59
  10, 60, 10, 10, 10, 10, 10, 60, 10, 10, // 60-69
  20, 10, 60, 10, 10, 10, 10, 10, 10, 10, // 70-79
  10, 10, 10, 10, 10, 10, 10, 10, 60, 10, // 80-89
  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 90-99
  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 100-109
  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 110-119
  10, 10, 10, 10, 10, 10, 10, 10          // 120-127
};


/*
 * CLASS DECLARATIONS
 */
 
class RectObstacle {
  private:
    RectObstacle();
  public:
    float x, y;
    byte w, h;
    float xSpeed, ySpeed;
    byte xd, yd;
    byte smooth;

    void smoothMove();
};

class Player {
  private:
    byte trickFC[5];
    
  public:
    // constructor
    Player();

    // variables
    byte id;
    byte x, y;
    byte padWidth, padHeight;
    byte roundsWon;
    byte health;
    float xSpeed, ySpeed;
    byte points;
    int roundsBarDir;
    byte roundsBarX;
    bool tricksMenuOn;
    byte tricks[5];
    bool trickOn[5];
    byte tricksCursor;
    byte selectedTrick; // peut-être à supprimer

    // functions
    void initPlayer(byte p);
    byte padTop(); // returns the top pixel y coordinate of the pad
    void drawPad();
    void moveUp();
    void moveDown();
    void updateTricksMenu();
    void updateTricks();
};

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

Ball ball;

extern const byte font3x3[];


/*
 * FUNCTION DECLARATIONS
 */

void drawBackground();
void masterWrite();

/*
 * SETUP
 */

void setup() {
  Serial.begin(9600);
  gb.begin();
  gb.display.setFont(font3x3);
  gb.titleScreen(F("PONG 2017"));
  gb.pickRandomSeed();
  gb.battery.show = false;
  for (byte i = 0; i < 2; i++) {
    player[i].initPlayer(i);
  }

}

/*
 * LOOP
 */
 
void loop() {
  if (gb.update()) {
    drawBackground();

    for (byte i = 0; i < 2; i++) player[i].updateTricks();

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

    if (gb.buttons.pressed(BTN_LEFT) && player[0].tricksMenuOn) {
      if (player[0].tricksCursor > 0) player[0].tricksCursor--;
      else player[0].tricksCursor = TRICKS_PER_PLAYER-1; 
    }
    if (gb.buttons.pressed(BTN_RIGHT) && player[0].tricksMenuOn) {
      if (player[0].tricksCursor < TRICKS_PER_PLAYER-1) player[0].tricksCursor++;
      else player[0].tricksCursor = 0;
    }     

    if (gb.buttons.pressed(BTN_B)) {
      player[0].tricksMenuOn = -player[0].tricksMenuOn+1;
    }

    if (gb.buttons.pressed(BTN_A) && player[0].tricksMenuOn) {
      gb.sound.playOK();
      player[0].selectedTrick = player[0].tricks[player[0].tricksCursor];
      player[0].tricksMenuOn = false;
      player[0].trickOn[player[0].tricksCursor] = true;      
    }
    
    if (gb.buttons.pressed(BTN_C)) {
      gb.titleScreen(F("PONG 2017"));
      gb.battery.show = false;
    }
  }
}
