static void menu2() { //waypoint
  if (button == 1 && m_set == 8) {
    button = 0;
    v_menu++;
  }
  if (v_menu > 6) { v_menu = 0; }
  if (button == 2 && m_set == 8) {
    button = 0;
    m_set = v_menu;
    if (m_set == 1) {
      steps = 1;
    } else {
      steps = 0;
    }
    if (m_set == 2) {
      long tlat = EEPROMReadlong(way_num * 20 + 10);
      if (tlat < 0) { 
        lat_temp[0] = 1; 
        tlat = -tlat;
      }
      lat_temp[10] = ( tlat               % 10) + 48;
      lat_temp[ 9] = ((tlat / 10)         % 10) + 48;
      lat_temp[ 8] = ((tlat / 100)        % 10) + 48;
      lat_temp[ 7] = ((tlat / 1000)       % 10) + 48;
      lat_temp[ 6] = ((tlat / 10000)      % 10) + 48;
      lat_temp[ 5] = ((tlat / 100000)     % 10) + 48;
      lat_temp[ 4] = ((tlat / 1000000)    % 10) + 48;
      lat_temp[ 3] = ((tlat / 10000000)   % 10) + 48;
      lat_temp[ 2] = ((tlat / 100000000)  % 10) + 48;
      lat_temp[ 1] = ((tlat / 1000000000) % 10) + 48;
    }
    if (m_set == 3) {
      long tlong = EEPROMReadlong(way_num * 20 + 15);
      if (tlong < 0) { 
        long_temp[0] = 1; 
        tlong = -tlong;
      }
      long_temp[10] = ( tlong               % 10) + 48;
      long_temp[ 9] = ((tlong / 10)         % 10) + 48;
      long_temp[ 8] = ((tlong / 100)        % 10) + 48;
      long_temp[ 7] = ((tlong / 1000)       % 10) + 48;
      long_temp[ 6] = ((tlong / 10000)      % 10) + 48;
      long_temp[ 5] = ((tlong / 100000)     % 10) + 48;
      long_temp[ 4] = ((tlong / 1000000)    % 10) + 48;
      long_temp[ 3] = ((tlong / 10000000)   % 10) + 48;
      long_temp[ 2] = ((tlong / 100000000)  % 10) + 48;
      long_temp[ 1] = ((tlong / 1000000000) % 10) + 48;
    }
  }
  if (button == 3 && m_set == 8) {
    screen = a_menu[2];
    button = 0;
  }
  u8g2.clearBuffer();
  header();
  u8g2.setCursor(0, 19);
  if (m_set == 0) {
    if (button == 1) {
      button = 0;
      way_num++;
      if (way_num > 20) { way_num = 1; }
    }
    if (button == 2) {
      button = 0;
      m_set = 8;
    }
    if (button == 3) {
      button = 0;
      way_num = 1;
    }
    u8g2.print("Number:");
    u8g2.setDrawColor(0);
    u8g2.printf("%2d", way_num);
    u8g2.setDrawColor(1);
    u8g2.setCursor(72, 19);
    u8g2.print("<");
  } else {
    u8g2.printf("Number:%2d%s", way_num, (v_menu == 0 ? "<" : " "));
  }
  if (m_set == 1) {
    if (button == 1) {
      button = 0;
      ch_temp[steps]++;
      if (ch_temp[steps] <  32) { ch_temp[steps] = 32; }
      if (ch_temp[steps] == 33) { ch_temp[steps] = 48; }
      if (ch_temp[steps] == 46) { ch_temp[steps] = 65; }
      if (ch_temp[steps] == 58) { ch_temp[steps] = 65; }
      if (ch_temp[steps] >  90) { ch_temp[steps] = 32; }
    }
    if (button == 2) {
      button = 0;
      m_set = 8;
      for (int x = 1; x < 5; x++) {
        EEPROM.write(way_num * 20 + 20 + x,ch_temp[x]);
      }
      steps = 1;
    }
    if (button == 3) {
      steps++;
      if (steps > 4) { steps = 1; }
    }
    u8g2.setCursor(96, 19);
    for (int x = 1; x < 5; x++) {
      if (steps == x) { u8g2.setDrawColor(0); }
      u8g2.print(ch_temp[x]);
      u8g2.setDrawColor(1);
    }
  } else {
    if (v_menu == 1) { u8g2.setDrawColor(0); }
    u8g2.setCursor(96, 19);
    for (int x = 1; x < 5; x++) {
      ch_temp[x] = EEPROM.read(way_num * 20 + 20 + x);
      u8g2.print(ch_temp[x]);
    }
    u8g2.setDrawColor(1);
  }
  if (m_set == 2) {
    if (button == 1) {
      button = 0;
      lat_temp[steps]++;
      if (steps == 0) {
        if (lat_temp[steps] >   1) { lat_temp[steps] =  0; }
      } else {
        if (lat_temp[steps] >  57) { lat_temp[steps] = 48; }
      }
    }
    if (button == 2) {
      button = 0;
      m_set = 8;
      steps = 0;
      long accum_lat = 0; 
      accum_lat = lat_temp[10] - 48; 
      accum_lat = accum_lat + (lat_temp[ 9] - 48)         * 10; 
      accum_lat = accum_lat + (lat_temp[ 8] - 48)        * 100; 
      accum_lat = accum_lat + (lat_temp[ 7] - 48)       * 1000; 
      accum_lat = accum_lat + (lat_temp[ 6] - 48)      * 10000; 
      accum_lat = accum_lat + (lat_temp[ 5] - 48)     * 100000; 
      accum_lat = accum_lat + (lat_temp[ 4] - 48)    * 1000000; 
      accum_lat = accum_lat + (lat_temp[ 3] - 48)   * 10000000; 
      accum_lat = accum_lat + (lat_temp[ 2] - 48)  * 100000000;
      accum_lat = accum_lat + (lat_temp[ 1] - 48) * 1000000000; 
      if (lat_temp[ 0]) { accum_lat = -accum_lat; }
      if (accum_lat == 0) {
      } else {
        EEPROMWritelong(way_num * 20 + 10,accum_lat);
      }
      lat_temp[0] = 0;
      for (int x = 1; x < 12; x++) { lat_temp[x] = 32; }
    }
    if (button == 3) {
      steps++;
      if (steps > 10) { steps = 0; }
    }
    u8g2.setCursor(0, 30);
    u8g2.print("Lat: ");
    for (int x = 0; x < 11; x++) {
      if (x > 0 && x < 3) {
        u8g2.drawBox(41 + (x * 8), 31, 8, 1);
      } else {
        u8g2.drawBox(41 + (x * 8), 31, 6, 1);
      }
      if (steps == x) { u8g2.setDrawColor(0); }
      if (x == 0 ) {
        u8g2.printf("%s", (lat_temp[0] == 1 ? "-" : " "));
      } else {
        u8g2.print(lat_temp[x]);
      }
      u8g2.setDrawColor(1);
    }
  } else {
    u8g2.setCursor(0, 30);
    u8g2.print("Lat: ");
    if (v_menu == 2) { u8g2.setDrawColor(0); }
    long tlat = EEPROMReadlong(way_num * 20 + 10);
    u8g2.printf("% 11d", tlat);
    u8g2.setDrawColor(1);
  }
  if (m_set == 3) {
    if (button == 1) {
      button = 0;
      long_temp[steps]++;
      if (steps == 0) {
        if (long_temp[steps] >   1) { long_temp[steps] =  0; }
      } else {
        if (long_temp[steps] >  57) { long_temp[steps] = 48; }
      }
    }
    if (button == 2) {
      button = 0;
      m_set = 8;
      steps = 0;
      long accum_long = 0; 
      accum_long = long_temp[10] - 48; 
      accum_long = accum_long + (long_temp[ 9] - 48)         * 10; 
      accum_long = accum_long + (long_temp[ 8] - 48)        * 100; 
      accum_long = accum_long + (long_temp[ 7] - 48)       * 1000; 
      accum_long = accum_long + (long_temp[ 6] - 48)      * 10000; 
      accum_long = accum_long + (long_temp[ 5] - 48)     * 100000; 
      accum_long = accum_long + (long_temp[ 4] - 48)    * 1000000; 
      accum_long = accum_long + (long_temp[ 3] - 48)   * 10000000; 
      accum_long = accum_long + (long_temp[ 2] - 48)  * 100000000; 
      accum_long = accum_long + (long_temp[ 1] - 48) * 1000000000; 
      if (long_temp[ 0]) { accum_long = -accum_long; }
      if (accum_long == 0) {
      } else {
        EEPROMWritelong(way_num * 20 + 15,accum_long);
      }
      long_temp[0] = 0;
      for (int x = 1; x < 12; x++) { long_temp[x] = 32; }
    }
    if (button == 3) {
      steps++;
      if (steps > 10) { steps = 0; }
    }
    u8g2.setCursor(0, 41);
    u8g2.print("Long:");
    for (int x = 0; x < 11; x++) {
      if (x > 0 && x < 3) {
        u8g2.drawBox(41 + (x * 8), 42, 8, 1);
      } else {
        u8g2.drawBox(41 + (x * 8), 42, 6, 1);
      }
      if (steps == x) { u8g2.setDrawColor(0); }
      if (x == 0 ) {
        u8g2.printf("%s", (long_temp[0] == 1 ? "-" : " "));
      } else {
        u8g2.print(long_temp[x]);
      }
      u8g2.setDrawColor(1);
    }
  } else {
    u8g2.setCursor(0, 41);
    u8g2.print("Long:");
    if (v_menu == 3) { u8g2.setDrawColor(0); }
    long tlong = EEPROMReadlong(way_num * 20 + 15);
    u8g2.printf("% 11d", tlong);
    u8g2.setDrawColor(1);
  }
  u8g2.setCursor(16, 52);
  u8g2.setDrawColor(1);
  if (v_menu == 4) { 
    u8g2.setDrawColor(0);
  }
  u8g2.printf("TRACKING %s", (track == way_num ? " ON" : "OFF"));
  u8g2.setDrawColor(1);
  if (m_set == 4) {
    if (track == way_num) { 
      track = 0;
      EEPROM.write(6,0);
    } else {
      track = way_num;
      EEPROM.write(6,track);
    }
    v_menu = 0;
    m_set = 8;
  }
  u8g2.setCursor(0, 63);
    u8g2.setDrawColor(0);
    u8g2.printf(" MARK%s  %sDELETE ", (v_menu == 5 ? "<" : " "), (v_menu == 6 ? ">" : " "));
    u8g2.setDrawColor(1);
    if (m_set == 5) {
      EEPROMWritelong(way_num * 20 + 10,fix_data.latitudeL());
      EEPROMWritelong(way_num * 20 + 15,fix_data.longitudeL());
      v_menu = 0;
      m_set = 8;
    }
    if (m_set == 6) {
      EEPROMWritelong(way_num * 20 + 10,0);
      EEPROMWritelong(way_num * 20 + 15,0);
      EEPROM.write(way_num * 20 + 21,45);
      EEPROM.write(way_num * 20 + 22,45);
      EEPROM.write(way_num * 20 + 23,45);
      EEPROM.write(way_num * 20 + 24,45);
      if (track == way_num) { 
        track = 0; 
        EEPROM.write(6,0);
      }
      v_menu = 0;
      m_set = 8;
    }
  u8g2.sendBuffer();
}



