static void menu7() { //sat view
  if (button == 3) {
    screen = a_menu[2];
    button = 0;
  }
  if (button == 1) {
    id++;
    button = 0;
  } 
  if (button == 2) {
    id = 0;
    button = 0;
  }
  if (id > 32) { id = 0; }
  if (refresh) {
    if (tsat < 1) {
      count++;
      if (count > 3) { count = 0; }
      refresh = 0;
      u8g2.clearBuffer();
      header();
      u8g2.setCursor(12, 30);
      u8g2.print("Searching for");
      u8g2.setCursor(24, 40);
      u8g2.print("satellites");
      u8g2.setCursor(44, 50);
      switch (count) {
      case 0:
        break;
      case 1:
        u8g2.print("  .  ");
        break;
      case 2:
        u8g2.print(" ... ");
        break;
      case 3:
        u8g2.print(".....");
        break;
      default:
      break;
    }
      u8g2.sendBuffer();
    } else {
      refresh = 0;
      u8g2.clearBuffer();
      header();
      u8g2.drawCircle(100, 36, 27, U8G2_DRAW_ALL);
      u8g2.drawLine(100, 34, 100, 38);
      u8g2.drawLine( 98, 36, 102, 36);
      for (int i = 1; i < 33; i++) {
        sat_id [i] = 0;
        sat_snr[i] = 0;
        sat_azi[i] = 0;
        sat_ele[i] = 0;
        sat_trk[i] = 0;
      }
      for (int i = 1; i < 33; i++) {
        int temp = gps.satellites[i].id;
        if (temp < 32) {
          sat_id [temp] = gps.satellites[i].id;
          sat_snr[temp] = gps.satellites[i].snr;
          sat_azi[temp] = gps.satellites[i].azimuth;
          sat_ele[temp] = gps.satellites[i].elevation;
          sat_trk[temp] = gps.satellites[i].tracked;
        }
      }
      compass.read();
      angle = compass.heading((LSM303::vector<int>){0, -1, 0});
      angle = angle - 270;
      if (angle < 0) { angle = angle + 360; }
      for (int x = 1; x < 33; x++) {
        if (sat_ele[x] && sat_azi[x]) {
          float head = sat_azi[x] - angle;
          if (head < 0) { head = head + 360; }
          point.fromPolar(map(sat_ele[x], 0, 90, 26, 0), head * PI / 180);
          tempx = point.getX() + 100;
          tempy = point.getY() + 36;
          int temp = map(sat_snr[x],0,100,1,30 );
          if (temp > 30) { temp = 30; }
          if (sat_trk[x]) {
            u8g2.drawDisc(tempx, tempy, 1, U8G2_DRAW_ALL);
            u8g2.drawBox(x * 2,61 - temp,2,temp);
          } else {
            u8g2.drawPixel(tempx, tempy);
            u8g2.drawBox(x * 2,61 - temp,1,temp);
          }
          if (id == x) {
            u8g2.drawCircle(tempx, tempy, 3, U8G2_DRAW_ALL);
          }
        }  
      }
      if (id != 0) {
        u8g2.setCursor(0, 18);
        u8g2.printf("ID:   %d", sat_id [id]);
        u8g2.setCursor(0, 26);
        u8g2.printf("SNR:  %d", sat_snr[id]);
        u8g2.drawBox(id * 2, 62, 2, 2);
      }
      
      u8g2.drawLine(2, 30, 64, 30);
      u8g2.sendBuffer();
    }
  }
}

