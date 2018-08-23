static void settings() { //settings
  if (refresh) {
    refresh = 0;
    u8g2.clearBuffer();
    header();
    if (button == 1 && m_set == 8) {
      button = 0;
      v_menu++;
    }
    if (v_menu > 7) { v_menu = 0; }
    if (button == 2 && m_set == 8) {
      button = 0;
      m_set = v_menu;
    }
    if (button == 3 && m_set == 8) {
      screen = a_menu[2];
      button = 0;
    }
    u8g2.setCursor(0, 15);       //Option 0 - GMT Hour Offset
    if (m_set == 0) {
      if (button == 1) {
        button = 0;
        a_menu[0]++;
      }
      if (button == 2) {
        button = 0;
        m_set = 8;
      }
      if (a_menu[0] > 11) { a_menu[0] = -11; }
      u8g2.print("Offset Hour ");
      u8g2.setDrawColor(0);
      u8g2.printf("%-+3d", a_menu[0]);
      u8g2.setDrawColor(1);
      u8g2.print("<");
    } else {
      u8g2.printf("Offset Hour %-+3d%s", a_menu[0], (v_menu == 0 ? "<" : " "));
    }
    u8g2.setCursor(0, 23);       //Option 1 - GMT Minute Offset
    if (m_set == 1) {
      if (button == 1) {
        button = 0;
        a_menu[1]++;
      }
      if (button == 2) {
        button = 0;
        m_set = 8;
      }
      if (a_menu[1] == 31) { a_menu[1] =  0; }
      if (a_menu[1] ==  1) { a_menu[1] = 30; }
      u8g2.print("Offset Mins ");
      u8g2.setDrawColor(0);
      u8g2.printf("%-2d",a_menu[1]);
      u8g2.setDrawColor(1);
      u8g2.print(" <");
    } else {
      u8g2.printf("Offset Mins %-2d %s", a_menu[1], (v_menu == 1 ? "<" : " "));
    }
    u8g2.setCursor(0, 31);       //Option 2 - Home Screen Selection
    if (m_set == 2) {
      if (button == 1) {
        button = 0;
        a_menu[2]++;
      }
      if (a_menu[2] > 8) { a_menu[2] = 1; }
      if (button == 2) {
        button = 0;
        m_set = 8;
      }
      u8g2.print("Home Screen ");
      u8g2.setDrawColor(0);
      u8g2.print(a_menu[2]);
      u8g2.setDrawColor(1);
      u8g2.print("  <");
    } else {
      u8g2.printf("Home Screen %d  %s", a_menu[2], (v_menu == 2 ? "<" : " "));
    }
    u8g2.setCursor(0, 39);       //Option 3 - Compass Source
    if (m_set == 3) {
      if (button == 1) {
        button = 0;
        a_menu[3]++;
        if (a_menu[3] > 2) { a_menu[3] = 0; }
      }
      if (button == 2) {
        button = 0;
        m_set = 8;
      }
      u8g2.print("Compass Src ");
      u8g2.setDrawColor(0);
      u8g2.printf("%s", (a_menu[3] == 0 ? "IC " : a_menu[3] == 1 ? "GPS" : "MIX"));
      u8g2.setDrawColor(1);
      u8g2.print("<");
    } else {
      u8g2.setDrawColor(1);
      u8g2.printf("Compass Src %s%s", (a_menu[3] == 0 ? "IC " : a_menu[3] == 1 ? "GPS" : "MIX"), 
                                      (v_menu == 3 ? "<" : " "));
    }
    u8g2.setCursor(0, 47);       //Option 4 - Reset waypoint memory
    if (m_set == 4) {
      if (button == 1) {
        button = 0;
        a_menu[4]++;
      }
      if (button == 2) {
        button = 0;
        m_set = 8;
        if (a_menu[4]) {
          a_menu[4] = 0;
          refresh = 1;
          EEPROM.write(6,0);
          a_menu[6] = 0;
          for (int x = 1;x < 21;x++) { 
            EEPROMWritelong(x * 20 + 10,0);
            EEPROMWritelong(x * 20 + 15,0);
            EEPROM.write(x * 20 + 21,45);
            EEPROM.write(x * 20 + 22,45);
            EEPROM.write(x * 20 + 23,45);
            EEPROM.write(x * 20 + 24,45);
          }  
        }
      }
      if (a_menu[4] > 1) { a_menu[4] = 0; }
      u8g2.print("Format Mem? ");
      u8g2.setDrawColor(0);
      u8g2.printf("%s", (a_menu[4] == 1 ? "YES" : "---"));
      u8g2.setDrawColor(1);
      u8g2.print("<");
    } else {
      u8g2.setDrawColor(1);
      u8g2.printf("Format Mem? %s%s", (a_menu[4] == 1 ? "YES" : "---"), (v_menu == 4 ? "<" : " "));
    }
    u8g2.setCursor(0, 55);       //Option 5 - Cold Start GPS Module
    if (m_set == 5) {
      if (button == 1) {
        button = 0;
        a_menu[5]++;
      }
      if (button == 2) {
        button = 0;
        m_set = 8;
        if (a_menu[5]) {
          a_menu[5] = 0;
          v_menu = 0;
          screen = a_menu[2];
          refresh = 1;
          updated = 1;
          GPS_PORT.end();
          Serial1.begin(115200);
          for (int x = 0; x < 12; x++) { Serial1.write(r_cold[x]); }
          Serial1.end();
          GPS_PORT.begin(115200);
          pinMode(PA9, INPUT);
        }
      }
      if (a_menu[5] > 1) { a_menu[5] = 0; }
      u8g2.print("Cold Start? ");
      u8g2.setDrawColor(0);
      u8g2.printf("%s", (a_menu[5] == 1 ? "YES" : "---"));
      u8g2.setDrawColor(1);
      u8g2.print("<");
    } else {
      u8g2.printf("Cold Start? %s%s", (a_menu[5] == 1 ? "YES" : "---"), (v_menu == 5 ? "<" : " "));
    }
    u8g2.setCursor(0, 63);       //Option 6 - Save Settings to Flash
    u8g2.setDrawColor(0);
    u8g2.printf("  SAVE%s  %sLOAD  ", (v_menu == 6 ? "<" : " "), (v_menu == 7 ? ">" : " "));
    u8g2.setDrawColor(1);
    
    if (m_set == 6) {
      if (a_menu[0] < 0 ) { a_menu[0] = a_menu[0] + 24; }
      for (int w = 0; w < 4; w++) {
        EEPROM.write(w,a_menu[w]);
      }
      if (a_menu[0] > 11) { a_menu[0] = a_menu[0] - 24; }
      v_menu = 0;
      m_set = 8;
    }
    if (m_set == 7) {
      for (int w = 0; w < 4; w++) { a_menu[w] = EEPROM.read(w); }
      if (a_menu[0] > 11) { a_menu[0] = a_menu[0] - 24; }
      v_menu = 0;
      m_set = 8;
    }
    u8g2.sendBuffer();
  }
}
