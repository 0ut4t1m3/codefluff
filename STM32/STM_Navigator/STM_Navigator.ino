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

//ublox binary commands for changing settings on the GPS receiver.
const int brd[28]     = {0xB5,0x62,0x06,0x00,0x14,0x00,0x01,0x00,0x00,0x00,0xD0,0x08,0x00,0x00,
                         0x00,0xC2,0x01,0x00,0x07,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0xC0,0x7E};
const int r_cold[12]  = {0xB5,0x62,0x06,0x04,0x04,0x00,0xFF,0xFF,0x02,0x00,0x0E,0x61};

bool mask1;	
bool mask2;
bool updated = 0;
bool refresh = 0;
bool lock;
bool lockout;
bool sat_trk[34]; 	//these arrays are for holding data on the satellite constellation
int sat_id[34];		//
int sat_snr[34];	//
int sat_azi[34];	//
int sat_ele[34];	//
int screen;
int button;
int a_menu[12] = {9,30,1,0,0,0,0,0,0,0,0,0}; //default system config, these values are overwritten with saved settings at boot
											 //{ hr offset, min offset, home screen, compass source, reserved, reserved, track target, reserved,...
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

//set up libraries
NeoGPS::Location_t waypoint;
Coordinates point = Coordinates();
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
LSM303 compass;
SeqButton button1_shr;
SeqButton button1_lng;
SeqButton button2_shr;
SeqButton button2_lng;


void setup()  {
  button1_shr.init(PB14, NULL, &button1_shrt, false, HIGH, 50);   //Set up buttons
  button1_lng.init(PB14, &button1_long, NULL, false, HIGH, 1000); //
  button2_shr.init(PB13, NULL, &button2_shrt, false, HIGH, 50);   //
  button2_lng.init(PB13, &button2_long, NULL, false, HIGH, 1000); //
  u8g2.begin();		//set up display
  u8g2.setFlipMode(0);
  u8g2.setContrast(255);
  u8g2.setPowerSave(0);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_chroma48medium8_8r);
  u8g2.setCursor(0, 8);
  u8g2.print("Booting...");
  u8g2.sendBuffer();
  Serial1.begin(9600);	//GPS module doesn't have eeprom so it defaults to 9600
  delay(100);
  for (int x = 0; x < 28; x++) { Serial1.write(brd[x]); } //send ublox binary command to switch to 115200
  delay(100);
  Serial1.end();	//close serial
  GPS_PORT.begin(115200); //reconnect to receiver
  pinMode(PA9, INPUT); //sets the serial TX pin to Hi-Z, this lets A PC send commands to the module through the external port
  Wire.begin();	//start compass setup, it takes time to settle so we'll come back to it
  compass.init();
  compass.enableDefault();
  pinMode(PB14, INPUT);		//these two lines can probably go? need to test first
  pinMode(PB13, INPUT);
  if (digitalRead(PB14) == 1 && digitalRead(PB13) == 1) {	//check if both buttons are being held down
    for (int w = 0; w < 12; w++) { 	//if they are
      EEPROM.write(w,a_menu[w]);	//Write default config to eeprom and continue
    }
  } else {	//and if they aren't
    for (int w = 0; w < 12; w++) { a_menu[w] = EEPROM.read(w); }	//load saved settings from eeprom
    if (a_menu[0] > 11) { a_menu[0] = a_menu[0] - 24; }				//reverses my hacky way of storing a negative time offset in a byte
  }
  compass.m_min = (LSM303::vector<int16_t>){-456, -598, -319};	//back to the compass, load calibration values
  compass.m_max = (LSM303::vector<int16_t>){+571, +417, +470};
  screen = a_menu[2];
  track  = a_menu[6];
  lat_temp[0] = 0;
  for (int x = 1; x < 12; x++) { lat_temp[x] = 32; }
  long_temp[0] = 0;
  for (int x = 1; x < 12; x++) { long_temp[x] = 32; }
}

void loop(void) { //program loop
  updated = 0;
  GPSloop();	//process incoming GPS data
  button = 0;
  button1_shr.handler();	//check if buttons are being held
  button1_lng.handler();
  button2_shr.handler();
  button2_lng.handler();
  switch (screen) {		//call routine for current view
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
      screen++;//skip, this page is empty
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

static void GPSloop() {		//GPS servicing loop
  while (gps.available( GPS_PORT )) { 
    fix_data = gps.read();
    updated = 1;
    refresh = 1;
  }
}

void header(bool x = 1) { //display header, shows current screen name and signal information
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

void DeFloat(float input) { //I like using printf but there's a bug when using floats on the STM32, this breaks one into two ints
  long calc = input * 100;
  df_whole = calc / 100;
  df_frac  = calc % 100;
}

void EEPROMWritelong(int address, long value) {		//splits a long int into 4 bytes for storing in eeprom and saves them
  byte four  = ( value        & 0xFF);
  byte three = ((value >> 8)  & 0xFF);
  byte two   = ((value >> 16) & 0xFF);
  byte one   = ((value >> 24) & 0xFF);
  EEPROM.write(address    , four );
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two  );
  EEPROM.write(address + 3, one  );
}

long EEPROMReadlong(long address) {					//reads four bytes in eeprom from the given offset and merges them into a long int
  long four  = EEPROM.read(address    );
  long three = EEPROM.read(address + 1);
  long two   = EEPROM.read(address + 2);
  long one   = EEPROM.read(address + 3);
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
