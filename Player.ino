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
  tricks[0] = EXPANDPAD;
  tricks[1] = BORDERS;
  tricks[2] = EXTRAPAD;
  tricks[3] = FREEZE;
  tricks[4] = INVISIBALL;
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
    for (byte i = 0; i < 5; i++) {
      gb.display.print(char(tricks[i]));
    }
  }
}

void Player::updateTrick() {
  if (trickOn) {
    if (trickFC == trickDuration[selectedTrick]) {
      padHeight = 7;
      trickOn = false;
      trickFC = 0;
      return;
    }
    switch (selectedTrick) {
      case EXPANDPAD:
        padHeight = 11;
        break;
    }
    trickFC++;
  }
}

