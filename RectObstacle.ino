RectObstacle::RectObstacle() {
  smooth = 8;
}

void RectObstacle::smoothMove() {
  xSpeed = (x-xd)/smooth;
  ySpeed = (y-yd)/smooth;
}

