#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;

#include <Wire.h>

// useless vars
int kjhk = 0;

boolean isMaster;
boolean paused = true;
boolean disconnected = false;
boolean slave_updated = false;

//I2C variables identifiers :
#define SLAVE_PAUSED 1
#define I_AM_MASTER 2
#define NAME 5
#define UP 10
#define DOWN 11
#define LEFT 12
#define RIGHT 13
#define BUT_A 14
#define BUT_B 15
#define SLAVE_DATA_BUFFER_LENGTH 7
#define NAME_LENGTH 3

char masterUp;
char masterDown;
char slaveUp;
char slaveDown;

char localName[4];
char masterName[4];
char slaveName[4] = { "   " };

void setupMaster();
void setupSlave();
void updateMaster();
void masterRead();
void masterWrite();
void requestSlaveName();
void sendSlaveName();

#define MENULENGTH 2
const char strMaster[] PROGMEM = "Host (master)";
const char strSlave[] PROGMEM = "Join (slave)";
const char* const menu[MENULENGTH] PROGMEM = {
  strMaster,
  strSlave
};

void setup() {
  Serial.begin(9600);
  gb.begin(); //initialize the Gamebuino
  gb.titleScreen(F("I2C test")); //shows the main menu
  gb.battery.show = false; //hide the battery indicator
  gb.keyboard(localName, 4);
  //can be either master or slave:
  setupMaster();
  setupSlave();
}

void loop() {

  paused = true;
  isMaster = false;

  switch(gb.menu(menu, MENULENGTH)){
  case 0: //Host
    paused = false;
    disconnected = false;
    isMaster = true;
    for (byte i = 0; i < 3; i++) {
      masterName[i] = localName[i];
    }
    break;
  case 1: //Join
    paused = false;
    disconnected = false;
    isMaster = false;
    for (byte i = 0; i < 3; i++) {
      slaveName[i] = localName[i];
    }
    break;
  default:
    gb.titleScreen(F("I2C test")); //shows the main menu
    break;
  }

  while (1) {
    if (gb.update()) {
      if(gb.buttons.pressed(BTN_C)){
        break;
      }
  
      if(isMaster){
        if (gb.buttons.repeat(BTN_UP, 1)) masterUp = 'U';
        else masterUp = ' ';
        gb.display.println(F("> Master <"));
        updateMaster();
      }
      else {
        if (gb.buttons.repeat(BTN_UP, 1)) slaveUp = 'U';
        else slaveUp = ' ';
        gb.display.println(F("> Slave <"));
        updateSlave();
      }
      updateDisplay();
    }
  }
}
