static void menu4() { //compass
  if (button == 3) {
    screen = a_menu[2];
    button = 0;
  }
  if (refresh) {
    if (track > 0 && fix_data.valid.location) {
      int32_t prevLat = EEPROMReadlong(track * 20 + 10); // code to retrieve a long integer for the latitude
      int32_t prevLon = EEPROMReadlong(track * 20 + 15); // code to retrieve a long integer for the longitude
      waypoint.lat( prevLat ); waypoint.lon( prevLon ); // set the lat/lon members of the base Location
      bearing = fix_data.location.BearingToDegrees(waypoint);
    }
    refresh = 0;
  }
  if (button == 3) {
    screen = a_menu[2];
    button = 0;
  }
  u8g2.clearBuffer();
  header();
  compass.read();
  angle = compass.heading((LSM303::vector<int>){0, -1, 0});
  int tangle = angle;
  angle = 0 - angle;
  if (angle >= 90) {
    angle = angle - 90;
  } else {
    angle = angle + 270;
  }
  point.fromPolar(22, angle * PI / 180);
  tempx = point.getX() + 60;
  tempy = point.getY() + 40;
  u8g2.setCursor(tempx, tempy);
  u8g2.print("N");
  point.fromPolar(22, (angle + 90) * PI / 180);
  tempx = point.getX() + 60;
  tempy = point.getY() + 40;
  u8g2.setCursor(tempx, tempy);
  u8g2.print("E");
  point.fromPolar(22, (angle + 180) * PI / 180);
  tempx = point.getX() + 60;
  tempy = point.getY() + 40;
  u8g2.setCursor(tempx, tempy);
  u8g2.print("S");
  point.fromPolar(22, (angle + 270) * PI / 180);
  tempx = point.getX() + 60;
  tempy = point.getY() + 40;
  u8g2.setCursor(tempx, tempy);
  u8g2.print("W");
  if (track > 0 && fix_data.valid.location) {
    float head = bearing + angle;
    if (head > 360) { head = head - 360; }
    point.fromPolar(22, head * PI / 180);
    tempx = point.getX() + 63;
    tempy = point.getY() + 36;
    u8g2.drawDisc(tempx, tempy, 2, U8G2_DRAW_ALL);
  }
  u8g2.setCursor(49, 40);
  u8g2.printf("%03d", tangle);
  u8g2.drawCircle(75, 35, 1, U8G2_DRAW_ALL);
  u8g2.drawCircle(63, 36, 27, U8G2_DRAW_ALL);
  u8g2.drawCircle(63, 36, 17, U8G2_DRAW_ALL);
  u8g2.drawLine(63, 23, 63, 30);
  u8g2.drawLine(60, 26, 62, 24);
  u8g2.drawLine(64, 24, 66, 26);
  u8g2.sendBuffer();
}

