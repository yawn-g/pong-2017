/*
 *    DRAW BACKGROUND
 */
void drawBackground() {
  byte yOffset;
  if (ball.y < 8 && hideHealth) yOffset = ball.y;
  else yOffset = 8;

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
  gb.display.cursorY = 2 - (8-yOffset);
  gb.display.print("YQN");
  gb.display.cursorX = 44;
  gb.display.print("RLF");

  // life gauges
  gb.display.fillRect(1, 2 - (8 -yOffset), 27, 3);
  gb.display.drawPixel(0, 3 - (8-yOffset));
  gb.display.fillRect(56, 2-(8-yOffset), 27, 3);
  gb.display.drawPixel(83, 3 - (8-yOffset));

  // rounds won
  for (byte p = 0; p < 2; p++) {
    for (byte i = 0; i < player[p].roundsWon; i++) {
      int dir = player[p].roundsBarDir;
      byte x = player[p].roundsBarX + i*4*dir;
      gb.display.drawFastHLine(x, 6 -(8-yOffset), 3);
    }    
  }

  // active tricks
  for (byte p = 0; p < 2; p++) {      // for each player
    gb.display.cursorY = 43;
    for (byte i = 0; i < 5; i++) {    // for each trick
      if (player[p].trickOn[i] == true) {
        gb.display.cursorX = 10 + i*4;
        gb.display.print(char(player[p].tricks[i]));
      }
    }
  }
}



