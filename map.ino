void startMap() {
  lcd.Fill_Rect(0, 0, lcdWidth, lcdHeight, CYAN);
  bool first = true;
  nowMap = 0;
  while(1)
  {
    if(first)
    {
      map();
      first = false;
    }

    if(isMapTouchDetected())
    {
      lcd.Fill_Rect(0, 0, lcdWidth, lcdHeight, CYAN);
      showMainMenu();
      break;
    }
  }
}

/* Map GUI */
// 맵 설정 display
void map() {
  lcd.Fill_Rect(0, 0, lcdWidth, lcdHeight, WHITE);
  lcd.Set_Text_Size(2); 
  lcd.Set_Text_colour(RED);

  lcd.Print("MAP CUSTOM", lcdWidth - 300, 20);
  lcd.Set_Draw_color(BLACK);
  lcd.Draw_Line(lcdWidth - 305, 40, 145, 40);

  lcd.Set_Draw_color(RED);
  lcd.Draw_Rectangle(250, 15, 301, 38);
  lcd.Fill_Rectangle(250, 15, 301, 38);
  showString("Back", 252, 20, 2, BLACK, true);

  showObs();
  selectObs();

}

void saveMapToEEPROM() {
  int address = 0;
  for (int i = 0; i < maxMapSize; i++) {
    EEPROM.put(address, mapSet[i]);
    address += sizeof(int8_t);
  }
  EEPROM.put(address, maxMapSize);
  address += sizeof(int8_t);
}

// 랭킹 데이터 읽기
void loadMapFromEEPROM() {
  int address = 0;
  for (int i = 0; i < maxMapSize; i++) {
    EEPROM.get(address, mapSet[i]);
    address += sizeof(int8_t);
  }
  EEPROM.get(address, maxMapSize);
  address += sizeof(int8_t);
}

// 맵 데이터 초기화 함수
void initMap() {
  for (int8_t i = 0; i < 100; ++i) {
    mapSet[i] = 0;
  }
  saveMapToEEPROM();
}

void showObs()
{
  touchObs = 0;
  if(nowMap == 0)
  {
    lcd.Set_Draw_color(WHITE);
    lcd.Fill_Rectangle(30, 100, 55, 120);
  }
  else
  {
    lcd.Set_Text_Size(2);
    lcd.Print("<<", 30, 100);
  }


  if(nowMap == maxMapSize-4)
  {
    lcd.Set_Draw_color(WHITE);
    lcd.Fill_Rectangle(260, 100, 285, 120);
  }
  else
  {
    lcd.Set_Text_Size(2);
    lcd.Print(">>", 260, 100);
  }

  lcd.Set_Draw_color(WHITE);
  lcd.Fill_Rectangle(90, 45, 232, 170);// 일단 지금 맵 지우고

  lcd.Set_Draw_color(BLACK);
  for(int8_t i = 0 ; i<5; i++)
  {
    switch(mapSet[nowMap+i])
    {
    case 1:
      lcd.Fill_Rectangle(i*30 + 92, 170, i*30 + 112, 140);
      break;
    case 2:
      lcd.Fill_Rectangle(i*30 + 92, 45, i*30 + 112, 150);
      break;
    case 3:
      lcd.Fill_Rectangle(i*30 + 92, 145, i*30 + 112, 125);
      break;
    }
  }
  selectObs();
  //saveMapToEEPROM();
  delay(200);
}

void selectObs()
{
  lcd.Set_Draw_color(BLACK);
  lcd.Draw_Line(85, 170, 237, 170);

  // 장애물 1
  lcd.Draw_Rectangle(30, 180, 70, 220);
  lcd.Fill_Rectangle(40, 221, 60, 190);
  //lcd.Fill_Triangle(40, 220, 60, 220, 50, 190);
  // 장애물 2
  lcd.Draw_Rectangle(85, 180, 125, 220);
  lcd.Fill_Rectangle(95, 180, 115, 210);
  //lcd.Fill_Triangle(95, 180, 115, 180, 105, 210);
  // 장애물 3
  lcd.Draw_Rectangle(140, 180, 180, 220);
  lcd.Fill_Rectangle(150, 190, 170, 210);

  lcd.Set_Draw_color(WHITE);
  lcd.Fill_Rectangle(260, 185, 290, 205);
  lcd.Print("Width: ",190, 185);
  lcd.Print_Number_Int(nowMap + 1, 260, 185, 3, '0', 10);

  lcd.Set_Draw_color(WHITE);
  lcd.Fill_Rectangle(250, 205, 290, 225);
  lcd.Print("Size: ", 190, 205);
  lcd.Print_Number_Int(maxMapSize + 1, 250, 205, 3, '0', 10);
  lcd.Print(">>", 290, 205);
}

/* Map GUI */

/* MapEdit Touch */
//맵 설정 터치
bool isMapTouchDetected() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
    p.x = map(p.x, TS_MAXX, TS_MINX, 0, lcd.Get_Display_Height());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, lcd.Get_Display_Width());

    if (p.y >= 248 && p.y <= 303) {
      if (p.x >= 12 && p.x <= 40) {
        return true;
      }
    }
  // << 선택 
    if (p.y >= 30 && p.y <= 90) {
      if (p.x >= 100 && p.x <= 160) {
        if(nowMap != 0)
        {
          nowMap--;
          showObs();
          delay(50);
        }
        return false;
      }
    }
    // >> 선택
    if (p.y >= 260 && p.y <= 320) {
      if (p.x >= 100 && p.x <= 160) {
        if(nowMap != maxMapSize-4)
        {
          nowMap++;
          showObs();
          delay(50);
        }
        return false;
      }
    }

    // 맵 사이즈 선택
    if (p.y >= 290 && p.y <= 320) {
      if (p.x >= 205 && p.x <= 240) {
        if(maxMapSize == 98)
          maxMapSize = 14;
        else if(maxMapSize == 14)
          maxMapSize = 29;
        else if(maxMapSize == 29)
          maxMapSize = 44;
        else
          maxMapSize = 98;

        if (nowMap > maxMapSize -4)
          nowMap = maxMapSize - 4;

        showObs();
        delay(50);
        return false;
      }
    }

    // 장애물1 30, 180, 70, 220
    if (p.y >= 30 && p.y <= 70) {
      if (p.x >= 180 && p.x <= 220) {
        if(touchObs != 1)
        {
          touchObs = 1;
          selectObs();
          lcd.Set_Draw_color(RED);
          lcd.Draw_Rectangle(30, 180, 70, 220);
          lcd.Fill_Rectangle(40, 221, 60, 190);
        }
        else
        {
          touchObs = 0;
          selectObs();
        }
        delay(250);
        return false;
      }
    }
    // 장애물2 85, 180, 125, 220
    if (p.y >= 85 && p.y <= 125) {
      if (p.x >= 180 && p.x <= 220) {
        if(touchObs != 2)
        {
          touchObs = 2;
          selectObs();
          lcd.Set_Draw_color(RED);
          lcd.Draw_Rectangle(85, 180, 125, 220);
          lcd.Fill_Rectangle(95, 180, 115, 210);
        }
        else
        {
          touchObs = 0;
          selectObs();
        }
        delay(250);
        return false;
      }
    }
    // 장애물3 140, 180, 180, 220
    if (p.y >= 140 && p.y <= 180) {
      if (p.x >= 180 && p.x <= 220) {
        if(touchObs != 3)
        {
          touchObs = 3;
          selectObs();
          lcd.Set_Draw_color(RED);
          lcd.Draw_Rectangle(140, 180, 180, 220);
          lcd.Fill_Rectangle(150, 190, 170, 210);
        }
        else
        {
          touchObs = 0;
          selectObs();
        }
        delay(250);
        return false;
      }
    }

    // 1번 설치
    if (p.y >= 92 && p.y <= 121) {
      if (p.x >= 45 && p.x <= 180) {
        mapSet[nowMap] = touchObs;
        showObs();
        return false;
      }
    }

  // 2번 설치
  if (p.y >= 122 && p.y <= 151) {
      if (p.x >= 45 && p.x <= 180) {
        mapSet[nowMap+1] = touchObs;
        showObs();
        return false;
      }
    }
  

  // 3번 설치
    if (p.y >= 152 && p.y <= 181) {
      if (p.x >= 45 && p.x <= 180) {
        mapSet[nowMap+2] = touchObs;
        showObs();
        return false;
      }
    }

  // 4번 설치
    if (p.y >= 182 && p.y <= 211) {
      if (p.x >= 45 && p.x <= 180) {
        mapSet[nowMap+3] = touchObs;
        showObs();
        return false;
      }
    }

  // 5번 설치
    if (p.y >= 212 && p.y <= 231) {
      if (p.x >= 45 && p.x <= 180) {
        mapSet[nowMap+4] = touchObs;
        showObs();
        return false;
      }
    }
    return false;
  }
  else return false;
}
/* MapEdit Touch */