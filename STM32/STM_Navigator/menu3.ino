static void menu3() { //navigation
  if (button == 3) {
    screen = a_menu[2];
    button = 0;
  }
  if (refresh) {
    refresh = 0;
    if (track > 0) {
      int32_t prevLat = EEPROMReadlong(track * 20 + 10); // code to retrieve a long integer for the latitude
      int32_t prevLon = EEPROMReadlong(track * 20 + 15); // code to retrieve a long integer for the longitude
      waypoint.lat( prevLat ); waypoint.lon( prevLon ); // set the lat/lon members of the base Location
    }
    u8g2.clearBuffer();
    header();
    if (track) {
      u8g2.setCursor(0, 19);
      u8g2.print("Tracking:");
      u8g2.setCursor(0, 30);
      for (int x = 1; x < 5; x++) {
        ch_temp[x] = EEPROM.read(track * 20 + 20 + x);
      }
      u8g2.printf("%2d.%c%c%c%c", track, ch_temp[1], ch_temp[2], ch_temp[3], ch_temp[4]);
      range = fix_data.location.DistanceKm(waypoint);
      bearing = fix_data.location.BearingToDegrees(waypoint);
      if (fix_data.valid.heading || a_menu[3] != 1) {
        switch (a_menu[3]) {
          case 0:
            compass.read();
            angle = compass.heading((LSM303::vector<int>){0, -1, 0});
            break;
          case 1:
            angle = fix_data.heading();
            break;
          case 2:
            if (fix_data.speed_kph() < 10.00) {
              compass.read();
              angle = compass.heading((LSM303::vector<int>){0, -1, 0});
            } else {
              angle = fix_data.heading();
            }
            break;
          default:
          break;
        }
        head = bearing - angle;
        if (head < 0) { head = head + 360; }
        u8g2.setCursor(0, 41);
        if (fix_data.valid.location) {
          int scale = 0;
          int pad = 4;
          if (range < 1.00) {
            DeFloat(range * 1000);
            scale = 1;
            pad = 3;
          } else {
            DeFloat(range);
            if (df_whole < 1000) { pad = 3; }
          }
          u8g2.printf("%*d.%02d%s", pad, df_whole, df_frac, (scale == 0 ? "km" : "m"));
        } else {
          u8g2.print(" INVALID");
        }
        u8g2.setCursor(0, 52);
        if (fix_data.valid.location) {
          DeFloat(bearing);
          u8g2.printf(" %3d.%02d", df_whole, df_frac);
        } else {
          u8g2.print(" ---.--");
        }
        u8g2.setCursor(89, 40);
        if (fix_data.valid.heading || a_menu[3] != 1) {
          if (fix_data.valid.location) {
            DeFloat(head);
            u8g2.printf("%03d", df_whole);
            if (head >= 90) {
              head = head - 90;
            } else {
              head = head + 270;
            }
            point.fromPolar(26, head * PI / 180);
            int x1 = point.getX() + 100;
            int y1 = point.getY() +  36;
            point.fromPolar(15, (head) * PI / 180);
            int x2 = point.getX() + 100;
            int y2 = point.getY() +  36;
            u8g2.drawLine(x1, y1, x2, y2);
          } else {
            u8g2.print("---");
          }
        } else {
          u8g2.print("---");
        }
      }
      u8g2.drawCircle(58, 47, 1, U8G2_DRAW_ALL);
      u8g2.drawCircle(100, 44, 1, U8G2_DRAW_ALL);
      u8g2.drawCircle(100, 36, 27, U8G2_DRAW_ALL);
      u8g2.drawCircle(100, 36, 14, U8G2_DRAW_ALL);
      u8g2.drawLine(100, 10, 100, 12); //N
      u8g2.drawLine(124, 36, 126, 36); //E
      u8g2.drawLine(100, 60, 100, 62); //S
      u8g2.drawLine( 74, 36, 76,  36); //W
      } else {
        u8g2.setCursor(0, 18);
        u8g2.print("Tracking:       ");
        u8g2.setCursor(36, 50);
        u8g2.print("OFFLINE");
      }
    u8g2.sendBuffer();  
  }
}
