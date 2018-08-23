#include <U8g2lib.h>
#include <SeqButton.h>
#include <NMEAGPS.h>
#include <Coordinates.h>
#include <EEPROM.h>
#include <Dusk2Dawn.h>
#include <Location.h>
#include <Wire.h>
#include <LSM303.h>

#define GPS_PORT Serial1
#define USING_GPS_PORT "Serial1"

const int brd[28]     = {0xB5,0x62,0x06,0x00,0x14,0x00,0x01,0x00,0x00,0x00,0xD0,0x08,0x00,0x00,
                         0x00,0xC2,0x01,0x00,0x07,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0xC0,0x7E};
const int r_cold[12]  = {0xB5,0x62,0x06,0x04,0x04,0x00,0xFF,0xFF,0x02,0x00,0x0E,0x61};

bool mask1;
bool mask2;
bool updated = 0;
bool refresh = 0;
bool sat_trk[34];
bool lock;
bool lockout;
int sat_id[34];
int sat_snr[34];
int sat_azi[34];
int sat_ele[34];
int screen;
int button;
int a_menu[12] = {9,30,1,0,0,0,0,0,0,0,0,0};
int v_menu = 0;
int m_set = 8;
int way_num = 1;
int track;
int steps = 1;
int tempx;
int tempy;
int tsat = 0;
int id;
int count;
long df_whole;
long df_frac;
float tfix;
float range;
float bearing;
float angle;
float head;
char ch_temp[5];
char lat_temp[12];
char long_temp[12];
static NMEAGPS  gps;
static gps_fix  fix_data;


NeoGPS::Location_t waypoint;
Coordinates point = Coordinates();
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
LSM303 compass;
SeqButton button1_shr;
SeqButton button1_lng;
SeqButton button2_shr;
SeqButton button2_lng;

void setup()  {
  button1_shr.init(PB14, NULL, &button1_shrt, false, HIGH, 50);   //
  button1_lng.init(PB14, &button1_long, NULL, false, HIGH, 1000); //
  button2_shr.init(PB13, NULL, &button2_shrt, false, HIGH, 50);   //
  button2_lng.init(PB13, &button2_long, NULL, false, HIGH, 1000); //
  u8g2.begin();
  u8g2.setFlipMode(0);
  u8g2.setContrast(255);
  u8g2.setPowerSave(0);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_chroma48medium8_8r);
  u8g2.setCursor(0, 8);
  u8g2.print("Booting...");
  u8g2.sendBuffer();
  Serial1.begin(9600);
  delay(100);
  for (int x = 0; x < 28; x++) { Serial1.write(brd[x]); }
  delay(100);
  Serial1.end();
  GPS_PORT.begin(115200);
  pinMode(PA9, INPUT);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  pinMode(PB14, INPUT);
  pinMode(PB13, INPUT);
  if (digitalRead(PB14) == 1 && digitalRead(PB13) == 1) {
    for (int w = 0; w < 12; w++) { 
      EEPROM.write(w,a_menu[w]);
    }
  } else {
    for (int w = 0; w < 12; w++) { a_menu[w] = EEPROM.read(w); }
    if (a_menu[0] > 11) { a_menu[0] = a_menu[0] - 24; }
  }
  compass.m_min = (LSM303::vector<int16_t>){-456, -598, -319};
  compass.m_max = (LSM303::vector<int16_t>){+571, +417, +470};
  screen = a_menu[2];
  track  = a_menu[6];
  lat_temp[0] = 0;
  for (int x = 1; x < 12; x++) { lat_temp[x] = 32; }
  long_temp[0] = 0;
  for (int x = 1; x < 12; x++) { long_temp[x] = 32; }
}

void loop(void) {
  updated = 0;
  GPSloop();
  button = 0;
  button1_shr.handler();
  button1_lng.handler();
  button2_shr.handler();
  button2_lng.handler();
  switch (screen) {
    case 1:
      menu1(); //mainview
      break;
    case 2:
      menu2(); //waypoint
      break;
    case 3:
      menu3(); //navigation
      break;
    case 4:
      menu4(); //compass
      break;
    case 5:
      screen++;
      break;
    case 6:
      menu6(); //sunview
      break;
    case 7:
      menu7(); //satview
      break;
    case 8:
      settings(); //settings
      break;
    default:
      screen = 1;
    break;
  }
}

static void GPSloop() {
  while (gps.available( GPS_PORT )) { 
    fix_data = gps.read();
    updated = 1;
    refresh = 1;
  }
}

void header(bool x = 1) {
  if (fix_data.valid.hdop && updated) { 
    tfix = fix_data.hdop / 1000.00; 
    tsat = fix_data.satellites;
  }
  u8g2.setCursor(0, 7);
  u8g2.setDrawColor(0);
  u8g2.printf("%d%c%s %c%-2d   ", screen, (lock == 1 ? 42 : 46), 
                                          (screen == 1 ? "GPS View" : screen == 2 ? "Waypoint" : 
                                           screen == 3 ? "Nav View" : screen == 4 ? "Compass " : 
                                           screen == 5 ? "Menu 5  " : screen == 6 ? "Sun Time" : 
                                           screen == 7 ? "Sat View" : screen == 8 ? "Settings" : 
                                           "????????"), 64, tsat);
                                          
  for (int x = 0; x < 5; x++) {
    u8g2.drawBox(113 + (x * 3), 6, 2, 1);
  }
  if (tfix < 10) { u8g2.drawBox(113, 5, 2, 1); }
  if (tfix <  5) { u8g2.drawBox(116, 4, 2, 2); }
  if (tfix <  3) { u8g2.drawBox(119, 3, 2, 3); }
  if (tfix <  2) { u8g2.drawBox(122, 2, 2, 4); }
  if (tfix <  1) { u8g2.drawBox(125, 1, 2, 5); }
  u8g2.setDrawColor(1);
}

void DeFloat(float input) {
  long calc = input * 100;
  df_whole = calc / 100;
  df_frac  = calc % 100;
}

void EEPROMWritelong(int address, long value) {
  byte four  = ( value        & 0xFF);
  byte three = ((value >> 8)  & 0xFF);
  byte two   = ((value >> 16) & 0xFF);
  byte one   = ((value >> 24) & 0xFF);
  EEPROM.write(address    , four );
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two  );
  EEPROM.write(address + 3, one  );
}

long EEPROMReadlong(long address) {
  long four  = EEPROM.read(address    );
  long three = EEPROM.read(address + 1);
  long two   = EEPROM.read(address + 2);
  long one   = EEPROM.read(address + 3);
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
