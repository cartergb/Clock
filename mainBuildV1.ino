/* 
Libraries that should be included:
Adafruit NeoPixel
RTClib by Adafruit
TFT_eSPI by Bodmer - requires User_Setup.h config

*/

/*
CONFIGURATIONS
Lights:
LIGHT = D2

RTC:




*/
#include <Adafruit_NeoPixel.h>
#include <RTClib.h>
#include <TFT_eSPI.h>
#include <SPI.h>


#define D0 44 //D0
#define D1 43 //D1
#define LIGHT 5 //D2
#define D3 6 //D3
#define D4 7 //D4
#define D5 8 //D5
#define D6 9 //D6
#define D7 10  //D7
#define D8 17 //D8
#define D9 18 //D9
#define D10 21 //D10 CS
#define D11 38 //D11 SDI(MOSI)
#define D12 47 //D12 (MISO)
#define D13 48 //D13 SCLK
#define Boot0 46 //Boot0
#define Boot1 0 //Boot1

#define A0 1 //A0 
#define A1 2 //A1 RST
#define A2 3 //A2 
#define A3 4 //A3
#define A4 11 //A4
#define A5 12 //A5
#define A6 13 //A6
#define A7 14 //A7


//neopixel config
#define NUMPIXELS 24 //number of LEDS

Adafruit_NeoPixel pixels(NUMPIXELS, LIGHT, NEO_GRB + NEO_KHZ800);

//RTC Config
RTC_DS3231 rtc;

// Display Config 
//(ST7796S via TFT_eSPI)
// Pin assignments are set in TFT_eSPI User_Setup.h (documents,arduino,libraries,TFT,user_setup.h )
TFT_eSPI tft = TFT_eSPI();

//Time tracking
int lastMinute = -1; //tracks last drawn minute to avoid flickering





//SETUP
void setup() {
  Serial.begin(115200);

  // NeoPixel
  pixels.begin();
  pixels.clear();
  pixels.show();

  // RTC
  if (!rtc.begin()) {
    Serial.println("ERROR: DS3231 RTC not found. Check I2C wiring (SDA/SCL).");
    while (true) delay(1000); // Halt — no point continuing without time
  }

  if (rtc.lostPower()) {
    Serial.println("WARNING: RTC lost power — time is not set. Setting to compile time.");
    // Sets RTC to the time this sketch was compiled — good enough for first boot
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //first time boot will call this code
  }

  // --- Display ---
  tft.init();
  tft.setRotation(1);           // Landscape — change to 0, 2, or 3 if needed - might be upside down on preview
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // Second arg = background, prevents ghosting

  drawClockFace(); // Draw static labels once
}


// =============================================
// LOOP
// =============================================
void loop() {

  // --- Update clock display once per minute ---
  DateTime now = rtc.now();
  if (now.minute() != lastMinute) {
    lastMinute = now.minute();
    drawTime(now);
  }

  // --- NeoPixel breathe ---
  pulseNeoPixel();
}





//DISPLAY Function
// Draws static parts of the clock face (only called once in setup)
void drawClockFace() {
  tft.fillScreen(TFT_BLACK);

  // Title label at top
  tft.setTextSize(2);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(10, 10);
  tft.print("CIRCADIAN CLOCK");

  // Divider line
  tft.drawFastHLine(0, 35, tft.width(), TFT_DARKGREY);
}

// Draws the current time — called whenever the minute changes
void drawTime(DateTime now) {
  // Format: HH:MM:SS
  char timeStr[9]; // "HH:MM:SS\0"
  snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d",
           now.hour(), now.minute(), now.second());

  // Format: Day, DD Mon YYYY
  char dateStr[20];
  const char* days[]   = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                           "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  snprintf(dateStr, sizeof(dateStr), "%s, %02d %s %04d",
           days[now.dayOfTheWeek()],
           now.day(),
           months[now.month() - 1],
           now.year());

  // --- Draw time (large, centered) ---
  tft.setTextSize(6);  // Adjust size to taste — ST7796S is 320x480 or 320x320
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  int timeX = (tft.width() - (8 * 6 * 6)) / 2; // Rough center: 8 chars * font width * size
  tft.setCursor(timeX > 0 ? timeX : 10, 80);
  tft.print(timeStr);

  // --- Draw date (smaller, below time) ---
  tft.setTextSize(2);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  int dateX = (tft.width() - (strlen(dateStr) * 6 * 2)) / 2;
  tft.setCursor(dateX > 0 ? dateX : 10, 180);
  tft.print(dateStr);
}



//LIGHT CODE  - Test, not actual code

void pulseNeoPixel() {
  // Pulse up: 0 to 127 over ~5 seconds
  for (int b = 0; b <= 127; b++) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(b, b, b));
    }
    pixels.show();
    delay(39);
  }

  // Pulse down: 127 to 0 over ~5 seconds
  for (int b = 127; b >= 0; b--) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(b, b, b));
    }
    pixels.show();
    delay(39);
  }
}









