static void menu1() { //main view
  if (button == 3) {
    screen = a_menu[2];
    button = 0;
  }
  if (refresh) {
    refresh = 0;
  int  zone_offset  =
                      a_menu[0] * NeoGPS::SECONDS_PER_HOUR +
                      a_menu[1] * NeoGPS::SECONDS_PER_MINUTE;
  NeoGPS::time_t localTime( fix_data.dateTime + zone_offset );
  int time_h = localTime.hours;
  int apm = 0;
  if (time_h == 12) {
    apm = 1;
  }
  if (time_h > 12) {
    time_h = time_h - 12;
    apm = 1;
  }
  if (time_h == 0) {
    time_h = 12;
    apm = 0;
  }
  u8g2.clearBuffer();
  header();
  u8g2.setCursor(0, 16);
  if ( fix_data.valid.time ) {
    u8g2.printf("Time: %2d:%02d:%02d%s", time_h, localTime.minutes, localTime.seconds, (apm == 1 ? "pm" : "am"));
  } else {
    u8g2.print("Time: --:--:--  ");
  }
  u8g2.setCursor(0, 24);
  if (fix_data.valid.date) {
    u8g2.printf("Date: %2d/%02d/20%02d", localTime.date, localTime.month, localTime.year);
  } else {
    u8g2.print("Date: --/--/----");
  }
  u8g2.setCursor(0, 32);
  if (fix_data.valid.location) { 
    u8g2.printf("Lat: % 11d", fix_data.latitudeL());
    u8g2.setCursor(0, 40);
    u8g2.printf("Lng: % 11d", fix_data.longitudeL());
  } else {
    u8g2.print("Lat:  ----------"); 
    u8g2.setCursor(0, 40);
    u8g2.print("Lng:  ----------"); 
  }
  u8g2.setCursor(0, 48);
  if (fix_data.valid.speed) {
    DeFloat(fix_data.speed_kph());
    u8g2.printf("Spd: %3d.%02dKm/H", df_whole, df_frac);
  } else {
    u8g2.print("Spd:   -.--Km/H");
  }
  u8g2.setCursor(0, 56);
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
    DeFloat(angle);
    u8g2.printf("Hdn: %3d.%02d", df_whole, df_frac);
    if (angle < 23)                  { u8g2.print(F(" N ")); }
    if (angle < 67  && angle >= 23)  { u8g2.print(F(" NE")); }
    if (angle < 112 && angle >= 67)  { u8g2.print(F(" E ")); }
    if (angle < 158 && angle >= 112) { u8g2.print(F(" SE")); }
    if (angle < 202 && angle >= 158) { u8g2.print(F(" S ")); }
    if (angle < 248 && angle >= 202) { u8g2.print(F(" SW")); }
    if (angle < 293 && angle >= 248) { u8g2.print(F(" W ")); }
    if (angle < 337 && angle >= 293) { u8g2.print(F(" NW")); }
    if (angle > 337)                 { u8g2.print(F(" N ")); }
  } else {
    u8g2.print("Hdn:   -.--   ");
  }
  u8g2.setCursor(0, 64);
  if (fix_data.valid.altitude) {
    int altf = fix_data.alt.frac;
    if (altf < 0) { altf = -altf; }
    u8g2.printf("Alt:% 4d.%02dM", fix_data.alt.whole, altf);
  } else {
    u8g2.print("Alt:   -.--M");
  }
  u8g2.sendBuffer();
  }
} // homeScreen
