static void menu6() { //sun times
  if (button == 3) {
    screen = a_menu[2];
    button = 0;
  }
  if (refresh) {
    refresh = 0;
    u8g2.clearBuffer();
    header();
    u8g2.setCursor(40, 19);
    u8g2.setCursor(0, 30);
    int  zone_offset  =
                        a_menu[0] * NeoGPS::SECONDS_PER_HOUR +
                        a_menu[1] * NeoGPS::SECONDS_PER_MINUTE;
    NeoGPS::time_t localTime( fix_data.dateTime + zone_offset );
    if ( fix_data.valid.time ) { 
      u8g2.printf("    Time: %2d:%02d", localTime.hours, localTime.minutes);
    } else {
      u8g2.print("    Time: --:--");
    }
    float temp = a_menu[0];
    if (a_menu[1] == 30) {
      if (temp >= 0) {
        temp = temp + 0.5;
      } else {
        temp = temp - 0.5;
      }
    }
    Dusk2Dawn sunTime(fix_data.latitude(), fix_data.longitude(), temp);
    int solRise  = sunTime.sunrise(localTime.year, localTime.month, localTime.date, false) + 35;
    int solSet   = sunTime.sunset (localTime.year, localTime.month, localTime.date, false) + 35;
    u8g2.setCursor(0, 41);
    if (fix_data.valid.location) {
      char solRise_v[] = "00:00";
      Dusk2Dawn::min2str(solRise_v, solRise);
      u8g2.printf(" Sunrise: %s",solRise_v);
    } else {
      u8g2.print(" Sunrise: --:--");
    }
    u8g2.setCursor(0, 52);
    if (fix_data.valid.location) {
      char solSet_v[] = "00:00";
      Dusk2Dawn::min2str(solSet_v, solSet);
      u8g2.printf("  Sunset: %s",solSet_v);
    } else {
      u8g2.print("  Sunset: --:--");
    }
    u8g2.setCursor(0, 63);
    if (fix_data.valid.location) {
      int solNoon = solRise + (solSet - solRise) / 2;
      char solNoon_v[] = "00:00";
      Dusk2Dawn::min2str(solNoon_v, solNoon);
      u8g2.printf(" Max Sol: %s",solNoon_v);
    } else {
      u8g2.print(" Max Sol: --:--");
    }
    u8g2.sendBuffer();
  }                             
}
