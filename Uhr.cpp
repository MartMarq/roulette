#include "Uhr.h"


RTC_DATA_ATTR int Welche = 1;

void Uhr::drawWatchFace() {

  // reset step counter at midnight
  if (currentTime.Hour == 00 && currentTime.Minute == 00) {
    sensor.resetStepCounter();
    //turn off radios
    WiFi.mode(WIFI_OFF);
    btStop();
  }

  //time adjustment start
  //if you don't need a time adjustment, just delete all lines between time adjustment start and end
  //my watchy runs too fast (15 seconds and 500 ms a day), this code set back time each day by 15 seconds and 500 ms

  if (currentTime.Hour == 00 && currentTime.Minute == 30) {

    RTC.read(currentTime);
    int8_t sekunde = currentTime.Second;
    int8_t minute = currentTime.Minute;
    int8_t hour = currentTime.Hour;
    int8_t day = currentTime.Day;
    int8_t month = currentTime.Month;
    int8_t year = tmYearToY2k(currentTime.Year);

    delay(15500);

    tmElements_t tm;
    tm.Month = month;
    tm.Day = day;
    tm.Year = y2kYearToTm(year);
    tm.Hour = hour;
    tm.Minute = minute;
    tm.Second = sekunde;

    RTC.set(tm);
  }
  // time adjustment end

  int stundeA = currentTime.Hour;
  int minuteA = currentTime.Minute;
  float VBAT = getBatteryVoltage();
  int n = 0;

  uint32_t Schritt = sensor.getCounter();
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  int16_t xc, yc, hour;
  uint16_t wc, hc;
  double winke, lange, cx1, cy1;

  int batLev;
  if (VBAT > 4) {
    batLev = 4;
  } else if (VBAT > 3.80 && VBAT <= 4) {
    batLev = 3;
  } else if (VBAT > 3.60 && VBAT <= 3.80) {
    batLev = 2;
  } else if (VBAT > 3.40 && VBAT <= 3.60) {
    batLev = 1;
  } else if (VBAT <= 3.40) {
    batLev = 0;
  }

  if (Welche == 2) {
    display.setFont(&FreeSans40pt7b);
    display.setCursor(0, 60);

    if (stundeA < 10) {
      display.print("0");
    }
    display.print(stundeA);
    display.print(":");
    if (minuteA < 10) {
      display.print("0");
    }
    display.println(minuteA);

    display.setFont(&FreeSans16pt7b);

    int16_t x1, y1;
    uint16_t w, h;

    String dayOfWeek = dayShortStr(currentTime.Wday);
    display.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);

    display.setCursor(50, 125);
    display.println(dayOfWeek);

    String month = monthShortStr(currentTime.Month);
    display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
    display.setCursor(50, 95);
    display.println(month);

    display.setCursor(5, 95);
    if (currentTime.Day < 10) {
      display.print("0");
    }
    display.println(currentTime.Day);

    display.drawBitmap(5, 175, steps, 19, 23, GxEPD_BLACK);
    display.drawBitmap(5, 145, akku, 30, 20, GxEPD_BLACK);
    display.setCursor(50, 195);
    display.println(Schritt);

    display.setCursor(50, 165);
    uint8_t Protz = min<int>(100 - ((4.20 - VBAT) * 100), 100);
    Protz = max<int>(Protz, 0);
    display.print(Protz);
    display.print(" %");

  } else {

    display.setFont(&FreeMonoBold8pt7b);

    for (n = 1; n <= 36; n = n + 3) {
      if (n == 13) {
        display.drawLine(8, 12 + (n * 5), 198, 12 + (n * 5), GxEPD_BLACK);
      }
      if (n == 25) {
        display.drawLine(8, 12 + (n * 5), 198, 12 + (n * 5), GxEPD_BLACK);
      }
      display.drawLine(58, 12 + (n * 5), 148, 12 + (n * 5), GxEPD_BLACK);
      display.getTextBounds(String(n), 100, 100, &xc, &yc, &wc, &hc);
      display.setCursor(70 - wc / 2, 28 + (n / 3 * 15));
      display.println(n);
      display.setCursor(100 - wc / 2, 28 + (n / 3 * 15));
      display.println(n + 1);
      display.setCursor(130 - wc / 2, 28 + (n / 3 * 15));
      display.println(n + 2);
    }
    n = 0;
    display.getTextBounds(String(n), 100, 100, &xc, &yc, &wc, &hc);
    display.setCursor(100 - wc / 2, 15);
    display.println(n);
    display.setFont(&FreeMonoBold7pt7b);
    display.setCursor(12, 47);
    display.print("PASSE");
    display.setCursor(150, 47);
    display.print("MANQUE");
    display.setCursor(14, 104);
    display.print("PAIR");
    display.setCursor(150, 104);
    display.print("IMPAIR");

    display.drawLine(58, 17, 103, 1, GxEPD_BLACK);
    display.drawLine(103, 1, 148, 17, GxEPD_BLACK);
    display.drawLine(58, 18, 58, 195, GxEPD_BLACK);
    display.drawLine(88, 18, 88, 195, GxEPD_BLACK);
    display.drawLine(118, 18, 118, 195, GxEPD_BLACK);
    display.drawLine(148, 18, 148, 195, GxEPD_BLACK);
    display.drawLine(8, 196, 198, 196, GxEPD_BLACK);
    display.drawLine(8, 195, 8, 17, GxEPD_BLACK);
    display.drawLine(198, 195, 198, 17, GxEPD_BLACK);
    display.drawLine(8, 17, 198, 17, GxEPD_BLACK);
    display.drawTriangle(33, 183, 23, 163, 43, 163, GxEPD_BLACK);
    display.drawTriangle(33, 143, 23, 163, 43, 163, GxEPD_BLACK);
    display.drawLine(24, 163, 42, 163, GxEPD_WHITE);
    display.fillTriangle(173, 183, 163, 163, 183, 163, GxEPD_BLACK);
    display.fillTriangle(173, 143, 163, 163, 183, 163, GxEPD_BLACK);


    if (stundeA == 0) {
      display.drawCircle(115, 11, 5, GxEPD_BLACK);
      display.drawCircle(115, 11, 3, GxEPD_BLACK);
    }
    for (n = 1; n <= 23; n = n + 3) {
      if (stundeA == n) {
        display.drawCircle(83, 24 + (n / 3 * 15), 5, GxEPD_BLACK);
        display.drawCircle(83, 24 + (n / 3 * 15), 3, GxEPD_BLACK);
      }
      if (stundeA == n + 1) {
        display.drawCircle(113, 24 + (n / 3 * 15), 5, GxEPD_BLACK);
        display.drawCircle(113, 24 + (n / 3 * 15), 3, GxEPD_BLACK);
      }
      if (stundeA == n + 2) {
        display.drawCircle(143, 24 + (n / 3 * 15), 5, GxEPD_BLACK);
        display.drawCircle(143, 24 + (n / 3 * 15), 3, GxEPD_BLACK);
      }
    }


    if (minuteA == 0) {
      display.fillCircle(87, 11, 5, GxEPD_BLACK);
    }
    wc = minuteA;
    if (wc > 36) {
      display.fillCircle(133, 160, 5, GxEPD_BLACK);
      wc = wc - 30;
    }

    for (n = 1; n <= 36; n = n + 3) {
      if (wc == n) {
        display.fillCircle(73, 24 + (n / 3 * 15), 5, GxEPD_BLACK);
      }
      if (wc == n + 1) {
        display.fillCircle(103, 24 + (n / 3 * 15), 5, GxEPD_BLACK);
      }
      if (wc == n + 2) {
        display.fillCircle(133, 24 + (n / 3 * 15), 5, GxEPD_BLACK);
      }
    }
    int tagA = currentTime.Day;

    for (n = 1; n <= 31; n = n + 3) {
      if (tagA >= n) {
        display.drawCircle(158, 24 + (n / 3 * 15), 5, GxEPD_BLACK);
        display.fillCircle(158, 24 + (n / 3 * 15), 4, GxEPD_WHITE);
        display.drawCircle(158, 24 + (n / 3 * 15), 3, GxEPD_BLACK);
      }
      if (tagA >= n + 1) {
        display.drawCircle(173, 24 + (n / 3 * 15), 5, GxEPD_BLACK);
        display.fillCircle(173, 24 + (n / 3 * 15), 4, GxEPD_WHITE);
        display.drawCircle(173, 24 + (n / 3 * 15), 3, GxEPD_BLACK);
      }
      if (tagA >= n + 2) {
        display.drawCircle(188, 24 + (n / 3 * 15), 5, GxEPD_BLACK);
        display.fillCircle(188, 24 + (n / 3 * 15), 4, GxEPD_WHITE);
        display.drawCircle(188, 24 + (n / 3 * 15), 3, GxEPD_BLACK);
      }
    }

    int monatA = currentTime.Month;

    for (n = 1; n <= 12; n = n + 3) {
      if (monatA >= n) {
        display.drawCircle(18, 24 + (n / 3 * 15), 5, GxEPD_BLACK);
        display.fillCircle(18, 24 + (n / 3 * 15), 4, GxEPD_WHITE);
        display.drawCircle(18, 24 + (n / 3 * 15), 3, GxEPD_BLACK);
      }
      if (monatA >= n + 1) {
        display.drawCircle(33, 24 + (n / 3 * 15), 5, GxEPD_BLACK);
        display.fillCircle(33, 24 + (n / 3 * 15), 4, GxEPD_WHITE);
        display.drawCircle(33, 24 + (n / 3 * 15), 3, GxEPD_BLACK);
      }
      if (monatA >= n + 2) {
        display.drawCircle(48, 24 + (n / 3 * 15), 5, GxEPD_BLACK);
        display.fillCircle(48, 24 + (n / 3 * 15), 4, GxEPD_WHITE);
        display.drawCircle(48, 24 + (n / 3 * 15), 3, GxEPD_BLACK);
      }
    }

    if (batLev >= 1) {
      display.drawCircle(18, 114, 5, GxEPD_BLACK);
      display.fillCircle(18, 114, 4, GxEPD_WHITE);
      display.drawCircle(18, 114, 3, GxEPD_BLACK);
    }
    if (batLev >= 2) {
      display.drawCircle(33, 114, 5, GxEPD_BLACK);
      display.fillCircle(33, 114, 4, GxEPD_WHITE);
      display.drawCircle(33, 114, 3, GxEPD_BLACK);
    }
    if (batLev >= 3) {
      display.drawCircle(48, 114, 5, GxEPD_BLACK);
      display.fillCircle(48, 114, 4, GxEPD_WHITE);
      display.drawCircle(48, 114, 3, GxEPD_BLACK);
    }
    if (batLev >= 4) {
      display.drawCircle(18, 130, 5, GxEPD_BLACK);
      display.fillCircle(18, 130, 4, GxEPD_WHITE);
      display.drawCircle(18, 130, 3, GxEPD_BLACK);
    }
  }
}


void Uhr::handleButtonPress() {
  uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
  if (IS_DOUBLE_TAP) {
    while (!sensor.getINT()) {
    }
  }


  if (IS_BTN_RIGHT_UP) {
    vibrate();
    Welche = Welche + 1;
    if (Welche > 2) {
      Welche = 1;
    }
    RTC.read(currentTime);
    drawWatchFace();
    showWatchFace(true);
  }
  if (IS_BTN_RIGHT_DOWN) {
  }

  Watchy::handleButtonPress();
}

void Uhr::vibrate() {

  sensor.enableFeature(BMA423_WAKEUP, false);
  pinMode(VIB_MOTOR_PIN, OUTPUT);

  delay(50);
  digitalWrite(VIB_MOTOR_PIN, true);
  delay(50);
  digitalWrite(VIB_MOTOR_PIN, false);

  sensor.enableFeature(BMA423_WAKEUP, true);
}
