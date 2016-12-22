/*
 *    CONSTRUCTOR
 */
Player::Player() {
  y = 23;
  padHeight = PAD_HEIGHT;
  padWidth = 2;
  health = HEALTH;
  roundsWon = 5;
  ySpeed = 2;
}

/*
 *    OTHER INIT
 */
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
  tricks[0] = EXPAND_PAD;
  tricks[1] = BORDERS;
  tricks[2] = EXTRA_PAD;
  tricks[3] = FREEZE;
  tricks[4] = INVISIBALL;
  tricksCursor = 2;
}


/*
 *    RETURNS TOP Y OF PAD
 */
byte Player::padTop() {
  return y - floor(padHeight/2);
}

/*
 *    DRAW PAD
 */
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

/*
 *    MOVE UPWARDS
 */
void Player::moveUp() {
  if (padTop() > 1) y -= ySpeed;
}

/*
 *    MOVE DOWNWARDS
 */
void Player::moveDown() {
  if (padTop() + padHeight < LCDHEIGHT-2) y += ySpeed;
}

/*
 *    UPDATE TRICKS MENU
 */
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

/*
 *    UPDATE TRICKS
 */
void Player::updateTricks() {
  // for each of the player's 5 tricks
  for (byte t = 0; t < 5; t++) {
    
    if (trickOn[t]) {
      
      // if duration elapsed
      if (trickFC[t] == trickDuration[tricks[t]]) {
        // restore states
        switch (tricks[t]) {
          case EXPAND_PAD:
            padHeight = PAD_HEIGHT;
            break;
          case INVISIBALL:
            ballVisible = true;
            break;
        }

        trickOn[t] = false;
        trickFC[t] = 0;
      }

      // if duration not elapsed
      else {
        switch (tricks[t]) {
          case EXPAND_PAD:
            padHeight = 11;
            break;
          case INVISIBALL:
            ballVisible = false;
            break;
        }
      }
      trickFC[t]++;
    }
  }
}

