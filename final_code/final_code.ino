/*
 ============================================================
       TME EDUCATION ANNIVERSARY CHATBOT
       Arduino UNO + VC-02 + SH1106 OLED
       TME Education Board LCD + NeoPixels
       MEMORY OPTIMIZED VERSION
 ============================================================
*/

#include <Wire.h>
#include <U8g2lib.h>
#include <SoftwareSerial.h>

#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#include <Adafruit_NeoPixel.h>


// ======================================================
// VC-02 UART
// ======================================================

// VC-02 TX -> Arduino D2
SoftwareSerial voiceSerial(2, 3);


// ======================================================
// OLED - PAGE BUFFER MODE
// ======================================================

// IMPORTANT:
// _1_ instead of _F_ greatly reduces SRAM usage.

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(
  U8G2_R0,
  U8X8_PIN_NONE
);


// ======================================================
// TME EDUCATION BOARD LCD
// ======================================================

const int LCD_ROWS = 2;
const int LCD_COLS = 16;

hd44780_I2Cexp lcd(
  0x20,
  I2Cexp_MCP23008,
  7, 6, 5, 4, 3, 2, 1,
  HIGH
);


// ======================================================
// TME EDUCATION BOARD NEOPIXELS
// ======================================================

#define LED_SERIAL 12
#define NUMPIXELS 5

Adafruit_NeoPixel pixels(
  NUMPIXELS,
  LED_SERIAL,
  NEO_GRB + NEO_KHZ800
);

unsigned long previousPixelMillis = 0;

const unsigned long PIXEL_INTERVAL = 40;

byte colorPosition = 0;


// ======================================================
// EYE SETTINGS
// ======================================================

#define LEFT_EYE_X   18
#define RIGHT_EYE_X  74
#define EYE_Y        14

#define EYE_WIDTH    36
#define EYE_HEIGHT   36
#define EYE_RADIUS   8


// ======================================================
// IDLE TIMER
// ======================================================

unsigned long lastIdleAction = 0;

const unsigned long IDLE_INTERVAL = 4000;


// ======================================================
// NEOPIXEL COLOR WHEEL
// ======================================================

uint32_t Wheel(byte position) {

  position = 255 - position;

  if (position < 85) {

    return pixels.Color(
      255 - position * 3,
      0,
      position * 3
    );
  }

  if (position < 170) {

    position -= 85;

    return pixels.Color(
      0,
      position * 3,
      255 - position * 3
    );
  }

  position -= 170;

  return pixels.Color(
    position * 3,
    255 - position * 3,
    0
  );
}


// ======================================================
// UPDATE NEOPIXELS
// ======================================================

void updateNeoPixels() {

  if (
    millis() - previousPixelMillis
    < PIXEL_INTERVAL
  ) {
    return;
  }

  previousPixelMillis = millis();

  for (byte i = 0; i < NUMPIXELS; i++) {

    byte position =
      colorPosition +
      (i * 50);

    pixels.setPixelColor(
      i,
      Wheel(position)
    );
  }

  pixels.show();

  colorPosition += 2;
}


// ======================================================
// DRAW NORMAL EYES
// ======================================================

void drawEyes(int eyeHeight, int pupilOffsetX = 0) {

  if (eyeHeight < 3)
    eyeHeight = 3;

  int y =
    EYE_Y +
    (EYE_HEIGHT - eyeHeight) / 2;


  u8g2.firstPage();

  do {

    // LEFT EYE

    u8g2.drawRBox(
      LEFT_EYE_X,
      y,
      EYE_WIDTH,
      eyeHeight,
      min(EYE_RADIUS, eyeHeight / 2)
    );


    // RIGHT EYE

    u8g2.drawRBox(
      RIGHT_EYE_X,
      y,
      EYE_WIDTH,
      eyeHeight,
      min(EYE_RADIUS, eyeHeight / 2)
    );


    // PUPILS

    if (eyeHeight > 15) {

      u8g2.setDrawColor(0);


      u8g2.drawDisc(
        LEFT_EYE_X +
        EYE_WIDTH / 2 +
        pupilOffsetX,

        y +
        eyeHeight / 2,

        6
      );


      u8g2.drawDisc(
        RIGHT_EYE_X +
        EYE_WIDTH / 2 +
        pupilOffsetX,

        y +
        eyeHeight / 2,

        6
      );


      u8g2.setDrawColor(1);
    }

  } while (u8g2.nextPage());
}


// ======================================================
// BLINK
// ======================================================

void blinkEyes() {

  // CLOSE

  for (int h = EYE_HEIGHT;
       h >= 3;
       h -= 5) {

    drawEyes(h);

    delay(20);
  }


  delay(70);


  // OPEN

  for (int h = 3;
       h <= EYE_HEIGHT;
       h += 5) {

    drawEyes(h);

    delay(20);
  }


  drawEyes(EYE_HEIGHT);
}


// ======================================================
// DOUBLE BLINK
// ======================================================

void doubleBlink() {

  blinkEyes();

  delay(120);

  blinkEyes();
}


// ======================================================
// LOOK LEFT
// ======================================================

void lookLeft() {

  for (int x = 0;
       x >= -7;
       x--) {

    drawEyes(
      EYE_HEIGHT,
      x
    );

    delay(25);
  }


  delay(300);


  for (int x = -7;
       x <= 0;
       x++) {

    drawEyes(
      EYE_HEIGHT,
      x
    );

    delay(25);
  }
}


// ======================================================
// LOOK RIGHT
// ======================================================

void lookRight() {

  for (int x = 0;
       x <= 7;
       x++) {

    drawEyes(
      EYE_HEIGHT,
      x
    );

    delay(25);
  }


  delay(300);


  for (int x = 7;
       x >= 0;
       x--) {

    drawEyes(
      EYE_HEIGHT,
      x
    );

    delay(25);
  }
}


// ======================================================
// HAPPY FACE
// ======================================================

void happyFace() {

  u8g2.firstPage();

  do {

    // LEFT EYE

    u8g2.drawRBox(
      18,
      12,
      36,
      30,
      8
    );


    // RIGHT EYE

    u8g2.drawRBox(
      74,
      12,
      36,
      30,
      8
    );


    // PUPILS

    u8g2.setDrawColor(0);

    u8g2.drawDisc(
      36,
      27,
      6
    );

    u8g2.drawDisc(
      92,
      27,
      6
    );

    u8g2.setDrawColor(1);


    // SMILE

    u8g2.drawLine(
      50, 48,
      54, 52
    );

    u8g2.drawLine(
      54, 52,
      59, 55
    );

    u8g2.drawLine(
      59, 55,
      64, 56
    );

    u8g2.drawLine(
      64, 56,
      69, 55
    );

    u8g2.drawLine(
      69, 55,
      74, 52
    );

    u8g2.drawLine(
      74, 52,
      78, 48
    );


  } while (u8g2.nextPage());
}


// ======================================================
// EXCITED FACE
// ======================================================

void drawExcitedFace() {

  u8g2.firstPage();

  do {

    // LARGE EYES

    u8g2.drawDisc(
      37,
      24,
      13
    );

    u8g2.drawDisc(
      91,
      24,
      13
    );


    // PUPILS

    u8g2.setDrawColor(0);

    u8g2.drawDisc(
      37,
      24,
      5
    );

    u8g2.drawDisc(
      91,
      24,
      5
    );

    u8g2.setDrawColor(1);


    // OPEN MOUTH

    u8g2.drawDisc(
      64,
      52,
      7
    );


  } while (u8g2.nextPage());
}


// ======================================================
// EXCITED ANIMATION
// ======================================================

void excitedFace() {

  for (byte i = 0;
       i < 2;
       i++) {

    drawExcitedFace();

    delay(250);

    happyFace();

    delay(250);
  }
}


// ======================================================
// FRIENDLY FACE
// ======================================================

void friendlyFace() {

  blinkEyes();

  happyFace();

  delay(900);

  drawEyes(EYE_HEIGHT);
}


// ======================================================
// CENTERED TEXT
// ======================================================

void showCenteredText(
  const char *line1,
  const char *line2,
  const char *line3
) {

  u8g2.firstPage();

  do {

    u8g2.setFont(
      u8g2_font_6x12_tf
    );


    int x;


    // LINE 1

    x =
      (128 -
       u8g2.getStrWidth(line1))
      / 2;

    u8g2.drawStr(
      x,
      18,
      line1
    );


    // LINE 2

    x =
      (128 -
       u8g2.getStrWidth(line2))
      / 2;

    u8g2.drawStr(
      x,
      38,
      line2
    );


    // LINE 3

    if (line3[0] != '\0') {

      x =
        (128 -
         u8g2.getStrWidth(line3))
        / 2;

      u8g2.drawStr(
        x,
        58,
        line3
      );
    }


  } while (u8g2.nextPage());
}


// ======================================================
// SHOW 10 YEARS
// ======================================================

void showTenYears() {

  excitedFace();


  u8g2.firstPage();

  do {

    // BIG 10

    u8g2.setFont(
      u8g2_font_logisoso24_tf
    );

    u8g2.drawStr(
      45,
      27,
      "10"
    );


    // YEARS OF

    u8g2.setFont(
      u8g2_font_7x13B_tf
    );

    u8g2.drawStr(
      28,
      45,
      "YEARS OF"
    );


    // TME EDUCATION

    u8g2.setFont(
      u8g2_font_6x12_tf
    );

    u8g2.drawStr(
      21,
      61,
      "TME EDUCATION"
    );


  } while (u8g2.nextPage());


  delay(2500);
}


// ======================================================
// EVENT DATE
// ======================================================

void showEventDate() {

  showCenteredText(
    "TME ANNIVERSARY",
    "26 / 09 / 2026",
    "SAVE THE DATE!"
  );

  delay(2500);
}


// ======================================================
// VENUE
// ======================================================

void showVenue() {

  showCenteredText(
    "VENUE",
    "NKUMBA",
    "UNIVERSITY"
  );

  delay(2500);
}


// ======================================================
// START TIME
// ======================================================

void showStartTime() {

  showCenteredText(
    "EVENT STARTS",
    "8:30 AM",
    "26 SEPT 2026"
  );

  delay(2500);
}


// ======================================================
// END TIME
// ======================================================

void showEndTime() {

  showCenteredText(
    "EVENT ENDS",
    "1:00 PM",
    "SEE YOU THERE!"
  );

  delay(2500);
}


// ======================================================
// DRAW ANNIVERSARY SCREEN
// ======================================================

void drawAnniversaryScreen(
  bool frame
) {

  u8g2.firstPage();

  do {

    // CONFETTI

    if (frame) {

      u8g2.drawDisc(
        8,
        8,
        2
      );

      u8g2.drawDisc(
        120,
        10,
        2
      );

      u8g2.drawDisc(
        14,
        52,
        2
      );

      u8g2.drawDisc(
        114,
        53,
        2
      );
    }

    else {

      u8g2.drawDisc(
        18,
        8,
        2
      );

      u8g2.drawDisc(
        108,
        10,
        2
      );

      u8g2.drawDisc(
        6,
        50,
        2
      );

      u8g2.drawDisc(
        122,
        48,
        2
      );
    }


    // TEXT

    u8g2.setFont(
      u8g2_font_7x13B_tf
    );

    u8g2.drawStr(
      27,
      23,
      "HAPPY 10TH"
    );


    u8g2.drawStr(
      19,
      42,
      "ANNIVERSARY"
    );


    u8g2.setFont(
      u8g2_font_6x12_tf
    );

    u8g2.drawStr(
      22,
      59,
      "TME EDUCATION"
    );


  } while (u8g2.nextPage());
}


// ======================================================
// ANNIVERSARY ANIMATION
// ======================================================

void anniversaryAnimation() {

  for (byte i = 0;
       i < 6;
       i++) {

    drawAnniversaryScreen(
      i % 2
    );

    delay(350);
  }
}


// ======================================================
// DRAW DANCER
// ======================================================

void drawDancer(
  int x,
  int y,
  bool pose
) {

  // HEAD

  u8g2.drawCircle(
    x,
    y,
    4
  );


  // BODY

  u8g2.drawLine(
    x,
    y + 4,
    x,
    y + 17
  );


  if (pose) {

    // ARMS UP

    u8g2.drawLine(
      x,
      y + 8,
      x - 8,
      y
    );

    u8g2.drawLine(
      x,
      y + 8,
      x + 8,
      y
    );


    // LEGS

    u8g2.drawLine(
      x,
      y + 17,
      x - 7,
      y + 27
    );

    u8g2.drawLine(
      x,
      y + 17,
      x + 5,
      y + 27
    );
  }

  else {

    // ARMS

    u8g2.drawLine(
      x,
      y + 8,
      x - 8,
      y + 14
    );

    u8g2.drawLine(
      x,
      y + 8,
      x + 8,
      y + 2
    );


    // LEGS

    u8g2.drawLine(
      x,
      y + 17,
      x - 3,
      y + 27
    );

    u8g2.drawLine(
      x,
      y + 17,
      x + 8,
      y + 23
    );
  }
}


// ======================================================
// DANCE FRAME
// ======================================================

void drawDanceFrame(
  byte frame
) {

  u8g2.firstPage();

  do {

    bool pose =
      frame % 2;


    // DANCERS

    drawDancer(
      24,
      16,
      pose
    );

    drawDancer(
      64,
      16,
      !pose
    );

    drawDancer(
      104,
      16,
      pose
    );


    // CONFETTI

    int c =
      frame * 7;


    u8g2.drawDisc(
      (c + 10) % 128,
      5,
      1
    );

    u8g2.drawDisc(
      (c + 45) % 128,
      10,
      1
    );

    u8g2.drawDisc(
      (c + 85) % 128,
      6,
      1
    );


    // TEXT

    u8g2.setFont(
      u8g2_font_6x12_tf
    );

    u8g2.drawStr(
      31,
      63,
      "CELEBRATE!"
    );


  } while (u8g2.nextPage());
}


// ======================================================
// FULL CELEBRATION
// ======================================================

void celebrationAnimation() {

  // Excited face

  excitedFace();

  delay(300);


  // Announcement

  showCenteredText(
    "IT'S 10 YEARS!",
    "LET'S",
    "CELEBRATE!"
  );

  delay(1200);


  // DANCE

  for (byte frame = 0;
       frame < 16;
       frame++) {

    drawDanceFrame(frame);

    delay(200);
  }


  // ANNIVERSARY

  anniversaryAnimation();


  // SLOGAN

  showCenteredText(
    "TOGETHER",
    "WE'LL GO",
    "FURTHER!"
  );

  delay(2200);


  // HAPPY ENDING

  happyFace();

  delay(1000);
}


// ======================================================
// GOODBYE
// ======================================================

void goodbyeAnimation() {

  happyFace();

  delay(700);


  showCenteredText(
    "THANK YOU!",
    "ENJOY THE",
    "CELEBRATION"
  );

  delay(1800);


  // CLOSE EYES

  for (int h = EYE_HEIGHT;
       h >= 3;
       h -= 4) {

    drawEyes(h);

    delay(30);
  }


  delay(500);


  // OPEN AGAIN

  blinkEyes();
}


// ======================================================
// PROJECT INFORMATION
// ======================================================

void projectAnimation() {

  showCenteredText(
    "HANDS-ON",
    "TECHNOLOGY",
    "IN ACTION!"
  );

  delay(1800);


  excitedFace();
}


// ======================================================
// IDLE ANIMATION
// ======================================================

void idleAnimation() {

  byte action =
    random(0, 4);


  switch (action) {

    case 0:

      blinkEyes();

      break;


    case 1:

      lookLeft();

      break;


    case 2:

      lookRight();

      break;


    case 3:

      doubleBlink();

      break;
  }


  drawEyes(EYE_HEIGHT);
}


// ======================================================
// PROCESS VC-02 COMMAND
// ======================================================

void processVoiceCommand(
  byte command
) {

  Serial.print(
    F("VC02: 0x")
  );


  if (command < 0x10) {

    Serial.print('0');
  }


  Serial.println(
    command,
    HEX
  );


  switch (command) {

    // 01 - HELLO

    case 0x01:

      happyFace();

      delay(1500);

      break;


    // 02 - WHO ARE YOU

    case 0x02:

      friendlyFace();

      break;


    // 03 - WHAT IS TME EDUCATION

    case 0x03:

      showCenteredText(
        "TME EDUCATION",
        "HANDS-ON",
        "LEARNING"
      );

      delay(2200);

      break;


    // 04 - WHAT ARE WE CELEBRATING

    case 0x04:

      anniversaryAnimation();

      break;


    // 05 - HOW OLD IS TME EDUCATION

    case 0x05:

      showTenYears();

      break;


    // 06 - WHEN IS CELEBRATION

    case 0x06:

      showEventDate();

      break;


    // 07 - WHERE IS CELEBRATION

    case 0x07:

      showVenue();

      break;


    // 08 - START TIME

    case 0x08:

      showStartTime();

      break;


    // 09 - END TIME

    case 0x09:

      showEndTime();

      break;


    // 0A - WHAT WILL HAPPEN

    case 0x0A:

      projectAnimation();

      break;


    // 0B - HAPPY ANNIVERSARY

    case 0x0B:

      anniversaryAnimation();

      break;


    // 0C - CELEBRATE

    case 0x0C:

      celebrationAnimation();

      break;


    // 0D - GOODBYE

    case 0x0D:

      goodbyeAnimation();

      break;
  }


  // RETURN TO NORMAL FACE

  drawEyes(EYE_HEIGHT);


  lastIdleAction =
    millis();
}


// ======================================================
// SETUP
// ======================================================

void setup() {

  // --------------------------------------------------
  // USB DEBUG
  // --------------------------------------------------

  Serial.begin(9600);


  // --------------------------------------------------
  // VC-02
  // --------------------------------------------------

  voiceSerial.begin(9600);


  // --------------------------------------------------
  // OLED
  // --------------------------------------------------

  u8g2.begin();


  // --------------------------------------------------
  // TME EDUCATION LCD
  // --------------------------------------------------

  lcd.begin(
    LCD_COLS,
    LCD_ROWS
  );

  lcd.clear();


  // Permanent anniversary message

  lcd.setCursor(3, 0);
  lcd.print("HAPPY 10TH");

  lcd.setCursor(1, 1);
  lcd.print("TME EDUCATION");


  // --------------------------------------------------
  // NEOPIXELS
  // --------------------------------------------------

  pixels.begin();

  // Bright but not excessive
  pixels.setBrightness(80);

  pixels.clear();

  pixels.show();


  // --------------------------------------------------
  // RANDOM GENERATOR
  // --------------------------------------------------

  randomSeed(
    analogRead(A0)
  );


  // --------------------------------------------------
  // ORIGINAL OLED STARTUP ANIMATION
  // --------------------------------------------------

  // START WITH CLOSED EYES
  // THEN WAKE UP

  for (int h = 3;
       h <= EYE_HEIGHT;
       h += 4) {

    drawEyes(h);

    delay(35);
  }


  blinkEyes();


  happyFace();

  delay(700);


  drawEyes(EYE_HEIGHT);


  lastIdleAction =
    millis();
}


// ======================================================
// MAIN LOOP
// ======================================================

void loop() {

  // ---------------------------------
  // CHANGING NEOPIXEL LIGHTS
  // ---------------------------------

  updateNeoPixels();


  // ---------------------------------
  // RECEIVE VC-02 COMMAND
  // ---------------------------------

  if (voiceSerial.available()) {

    byte command =
      voiceSerial.read();


    processVoiceCommand(
      command
    );
  }


  // ---------------------------------
  // IDLE ANIMATION
  // ---------------------------------

  if (
    millis() -
    lastIdleAction
    >=
    IDLE_INTERVAL
  ) {

    idleAnimation();


    lastIdleAction =
      millis();
  }
}