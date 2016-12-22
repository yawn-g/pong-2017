Ball::Ball() {
  x = 41;
  y = 23;
  xSpeed = 1;
  ySpeed = 1;
  width = 2;
}

void Ball::draw() {
  if (ballVisible) {
    gb.display.setColor(BLACK);
    gb.display.fillRect(x, y, width, width);
  }
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

