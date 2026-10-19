void startUser() {
  lcd.Fill_Rect(0, 0, lcdWidth, lcdHeight, CYAN);
  bool first = true;
  while(1)
  {
    if(first)
    {
      user();
      first = false;
    }

    if(isUserTouchDetected())
    {
      lcd.Fill_Rect(0, 0, lcdWidth, lcdHeight, CYAN);
      showMainMenu();
      break;
    }
  }

}

/* User GUI */
// 캐릭터 선택
void nowChoiceUser(int16_t x, int16_t y, int16_t disp)
{
  //캐릭터 지우기
  if(disp == 0)
  {
    lcd.Set_Draw_color(CYAN);
    nowDelUser(x,y);
  }
  else if(disp == 1)
  {
    lcd.Set_Draw_color(MAGENTA);
    nowDelUser(x,y);
  }

  switch(types)
  {
    case 0: // type가 0이면  거북이
      turtle(x, y, 2, true, false);
      break;
    case 1:// type가 1이면  젖소
      panda(x, y, 2, true, false);
      break;
    case 2:// type가 2이면  코끼리
      elephant(x, y, 2, true, false);
      break;
  }
}

void user() {
  lcd.Fill_Rect(0, 0, lcdWidth, lcdHeight, MAGENTA);
  lcd.Set_Text_Size(2); 
  lcd.Set_Text_colour(BLUE);

  lcd.Print("USER CUSTOM", lcdWidth - 300, 20);
  lcd.Set_Draw_color(BLACK);
  lcd.Draw_Line(lcdWidth - 305, 40, 155, 40);

  lcd.Set_Draw_color(RED);
  lcd.Draw_Rectangle(250, 15, 301, 38);
  lcd.Fill_Rectangle(250, 15, 301, 38);
  showString("Back", 252, 20, 2, BLACK, true);
  
  lcd.Set_Text_Size(2);
  lcd.Print("<<", 30, 100);
  lcd.Print(">>", 260, 100);

  lcd.Set_Draw_color(BLACK);
  lcd.Draw_Line(20, 155, 298, 155);

  lcd.Set_Draw_color(RED);
  lcd.Fill_Circle(48, 190, 25);

  lcd.Set_Draw_color(BLUE);
  lcd.Fill_Circle(103, 190, 25);

  lcd.Set_Draw_color(APRICOT);
  lcd.Fill_Circle(158, 190, 25);

  lcd.Set_Draw_color(GREEN);
  lcd.Fill_Circle(213, 190, 25);

  lcd.Set_Draw_color(BLACK);
  lcd.Fill_Circle(268, 190, 25);

  nowChoiceUser(130, 80, 1);
}

// 큰 캐릭터 삭제
void nowDelUser(int16_t x,int16_t y) { 
  lcd.Fill_Rectangle(x, y, x + 50, y + 50); // 몸통  
  lcd.Fill_Rectangle(x + 50, y + 6, x + 50 + 24, y + 6 + 24); // 머리
  lcd.Fill_Rectangle(x + 50 + 18, y + 6 + 24, x + 50 + 24, y + 54); // 코
  lcd.Fill_Rectangle(x, y + 22 + 28, x + 10, y + 22 + 28 + 10);// 뒷다리
  lcd.Fill_Rectangle(x + 40, y + 22 + 28, x + 50, y + 22 + 28 + 10); // 앞다리
  lcd.Fill_Rectangle(x + 50 + 18, y + 12, x + 50 + 20, y + 12 + 8); // 눈
  lcd.Fill_Rectangle(x + 50 + 8, y, x + 50 + 12, y + 6); // 귀
}
/* User GUI */

/* UserEdit Touch */
bool isUserTouchDetected() {
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
        if(types == 0)
          types = 2;
        else
          types -= 1;
        nowChoiceUser(130, 80, 1);
        delay(500);
        return false;
      }
    }
    // >> 선택
    if (p.y >= 260 && p.y <= 320) {
      if (p.x >= 100 && p.x <= 160) {
        if(types == 2)
          types = 0;
        else
          types += 1;
        nowChoiceUser(130, 80, 1);
        delay(500);
        return false;
      }
    }
    // 빨간색 RED
    if (p.y >= 23 && p.y <= 83) {
      if (p.x >= 165 && p.x <= 215) {
        colors = RED;
        nowChoiceUser(130, 80, 1);
        delay(500);
        return false;
      }
    }
    // 파란색 BLUE
    if (p.y >= 78 && p.y <= 128) {
      if (p.x >= 165 && p.x <= 215) {
        colors = BLUE;
        nowChoiceUser(130, 80, 1);
        delay(500);
        return false;
      }
    }
    // 황색 APRICOT
    if (p.y >= 123 && p.y <= 173) {
      if (p.x >= 165 && p.x <= 215) {
        colors = APRICOT;
        nowChoiceUser(130, 80, 1);
        delay(500);
        return false;
      }
    }
    // 초록색 GREEN
    if (p.y >= 188 && p.y <= 238) {
      if (p.x >= 165 && p.x <= 215) {
        colors = GREEN;
        nowChoiceUser(130, 80, 1);
        delay(500);
        return false;
      }
    }
    // 회색 GRAY
    if (p.y >= 243 && p.y <= 393) {
      if (p.x >= 165 && p.x <= 215) {
        colors = GRAY;
        nowChoiceUser(130, 80, 1);
        delay(500);
        return false;
      }
    }
    
    return false;
  }
  else return false;
}
/* UserEdit Touch */