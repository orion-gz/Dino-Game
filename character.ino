// 현재 설정된 캐릭터 출력 함수
void choiceUser(int16_t x, int16_t y, bool isJump, bool isSlide)
{
  switch(types)
  {
    case 0:
      turtle(x, y, 1, isJump, isSlide);
      break;
    case 1:
      panda(x, y, 1, isJump, isSlide);
      break;
    case 2:
      elephant(x, y, 1, isJump, isSlide);
      break;
  }
}

// 거북이
void turtle(int x, int y, int sized, bool isJump, bool isSlide) { // 몸통 히트박스 x<= Ax <= x+25, y <=Ax <= 30 , 머리 히트박스 x+25<= Ax <=x+37, y+3<=Ax<=y+15
  unsigned long currentMillis = millis();
  
  lcd.Set_Draw_color(colors);
  if (isSlide) lcd.Fill_Rectangle(x, y + 4, x + 25, y + 11);
  else lcd.Fill_Rectangle(x, y, x + 25*sized, y + 11*sized); // 껍질
  
  lcd.Set_Draw_color(YELLOW); 
  if (isSlide) lcd.Fill_Rectangle(x, y + 11, x + 25, y + 11 + 5); // 몸통  
  else lcd.Fill_Rectangle(x, y + 11*sized, x + 25*sized, y + 25*sized); // 몸통  
  lcd.Fill_Rectangle(x + 25*sized, y + 3*sized, x + 37*sized, y + 15*sized); // 머리  
  
  lcd.Set_Draw_color(BLACK); 
  lcd.Fill_Rectangle(x + 34*sized, y + 6*sized, x + 35*sized, y + 10*sized); // 눈
  if (isSlide) return;
  lcd.Set_Draw_color(BACKGROUND); // 다리 삭제
  if (sized == 1) lcd.Fill_Rectangle(x, y + 25, x + 25, y + 30);

  if(isJump)
  {
    lcd.Set_Draw_color(YELLOW);
    lcd.Fill_Rectangle(x, y + 25*sized, x + 5*sized, y + 30*sized); // 뒷다리
    lcd.Fill_Rectangle(x + 20*sized, y + 25*sized, x + 25*sized, y + 30*sized); // 앞다리
    return;
  }

  
  lcd.Set_Draw_color(YELLOW);
  if (currentMillis%320 < 80) {
    // 뒷다리
    lcd.Fill_Rectangle(x, y + 25, x + 5, y + 30);
    // 앞다리
    lcd.Fill_Rectangle(x + 20, y + 25, x + 25, y + 30);
  } 
  else if (currentMillis%320 >= 80 && currentMillis%320 < 160) {
    // 뒷다리
    lcd.Fill_Rectangle(x + 2, y + 11 + 14, x + 7, y + 11 + 14 + 5);
    // 앞다리
    lcd.Fill_Rectangle(x + 18, y + 11 + 14, x + 23, y + 11 + 14 + 5);
  } 
  else if (currentMillis%320 >= 160 && currentMillis%320 < 240) {
    // 뒷다리
    lcd.Fill_Rectangle(x + 4, y + 11 + 14, x + 9, y + 11 + 14 + 5);
    // 앞다리
    lcd.Fill_Rectangle(x + 16, y + 11 + 14, x + 21, y + 11 + 14 + 5);
  } 
  else if (currentMillis%320 >= 240) {
    // 뒷다리
    lcd.Fill_Rectangle(x + 2, y + 11 + 14, x + 7, y + 11 + 14 + 5);
    // 앞다리
    lcd.Fill_Rectangle(x + 18, y + 11 + 14, x + 23, y + 11 + 14 + 5);
  }
}
//판다
void panda(int x, int y, int sized, bool isJump, bool isSlide) {
  unsigned long currentMillis = millis();
  // 판다 몸통
  lcd.Set_Draw_color(colors);
  if (isSlide) lcd.Fill_Rectangle(x, y + 4, x + 7*sized, y + 8*sized); // 등
  else lcd.Fill_Rectangle(x, y, x + 7*sized, y + 11*sized); // 등

  if (isSlide) lcd.Fill_Rectangle(x + 13*sized, y + 4, x + 25*sized, y + 8*sized); // 등
  else lcd.Fill_Rectangle(x + 13*sized, y, x + 25*sized, y + 11*sized); // 등

  lcd.Set_Draw_color(WHITE);
  if (isSlide) lcd.Fill_Rectangle(x + 7*sized, y + 4, x + 13*sized, y + 8*sized); // 등
  else lcd.Fill_Rectangle(x + 7*sized, y, x + 13*sized, y + 11*sized); // 등
  
  lcd.Set_Draw_color(WHITE);

  if (isSlide) {
    lcd.Fill_Rectangle(x, y + 8*sized, x + 25*sized, y + 11*sized); // 몸통  
    lcd.Fill_Rectangle(x, y + 11*sized, x + 13*sized, y + 16*sized); // 몸통 배
  }
  else { 
    lcd.Fill_Rectangle(x, y + 11*sized, x + 25*sized, y + 18*sized); // 몸통  
    lcd.Fill_Rectangle(x, y + 18*sized, x + 13*sized, y + 25*sized); // 몸통 배
  }

  lcd.Set_Draw_color(colors);

  if (isSlide) lcd.Fill_Rectangle(x + 13*sized, y + 11*sized, x + 17*sized, y + 16*sized); // 몸통 배
  else lcd.Fill_Rectangle(x + 13*sized, y + 18*sized, x + 17*sized, y + 25*sized); // 몸통 배

  lcd.Set_Draw_color(WHITE);
  lcd.Fill_Rectangle(x + 25*sized, y + 3*sized, x + 31*sized, y + 13*sized); // 머리
  lcd.Fill_Rectangle(x + 25*sized, y + 13*sized, x + 37*sized, y + 15*sized); // 머리

  if (isSlide) lcd.Fill_Rectangle(x + 17*sized, y + 11*sized, x + 25*sized, y + 16*sized); // 몸통 배
  else lcd.Fill_Rectangle(x + 17*sized, y + 18*sized, x + 25*sized, y + 25*sized); // 몸통 배
  
  lcd.Set_Draw_color(colors);
  lcd.Fill_Rectangle(x + 29*sized, y, x + 31*sized, y + 3*sized); // 귀
  lcd.Fill_Rectangle(x + 31*sized, y + 3*sized, x + 37*sized, y + 13*sized); // 머리

  lcd.Set_Draw_color(WHITE);
  lcd.Fill_Rectangle(x + 34*sized, y + 6*sized, x + 35*sized, y + 8*sized); // 눈
  if (isSlide) return;

  lcd.Set_Draw_color(BACKGROUND); // 다리 삭제
  if (sized == 1) lcd.Fill_Rectangle(x-2, y + 11 + 14, x + 27, y + 11 + 14 + 5);
  
  if(isJump) {
    lcd.Set_Draw_color(colors);
    lcd.Fill_Rectangle(x, y + 25*sized, x + 5*sized, y + 30*sized); // 뒷다리
    lcd.Fill_Rectangle(x + 20*sized, y + 25*sized, x + 25*sized, y + 30*sized); // 앞다리
    return;
  }
  
  lcd.Set_Draw_color(colors);
  if (currentMillis%320 < 80) {
    // 뒷다리
    lcd.Fill_Rectangle(x, y + 25, x + 5, y + 30);
    // 앞다리
    lcd.Fill_Rectangle(x + 20, y + 25, x + 25, y + 30);
  } 
  else if (currentMillis%320 >= 80 && currentMillis%320 < 160) {
    // 뒷다리
    lcd.Fill_Rectangle(x + 2, y + 11 + 14, x + 7, y + 11 + 14 + 5);
    // 앞다리
    lcd.Fill_Rectangle(x + 18, y + 11 + 14, x + 23, y + 11 + 14 + 5);
  } 
  else if (currentMillis%320 >= 160 && currentMillis%320 < 240) {
    // 뒷다리
    lcd.Fill_Rectangle(x + 4, y + 11 + 14, x + 9, y + 11 + 14 + 5);
    // 앞다리
    lcd.Fill_Rectangle(x + 16, y + 11 + 14, x + 21, y + 11 + 14 + 5);
  } 
  else if (currentMillis%320 >= 240) {
    // 뒷다리
    lcd.Fill_Rectangle(x + 2, y + 11 + 14, x + 7, y + 11 + 14 + 5);
    // 앞다리
    lcd.Fill_Rectangle(x + 18, y + 11 + 14, x + 23, y + 11 + 14 + 5);
  }
}

//코끼리
void elephant(int x, int y, int sized, bool isJump, bool isSlide) { // 몸통 히트박스 x<= Ax <= x+25, y <=Ax <= 30 , 머리 히트박스 x+25<= Ax <=x+37, y+3<=Ax<=y+15
  unsigned long currentMillis = millis();
  // 코끼리
  lcd.Set_Draw_color(colors);
  if (isSlide) lcd.Fill_Rectangle(x, y + 4, x + 25, y + 16);
  else lcd.Fill_Rectangle(x, y, x + 25*sized, y + 25*sized); // 몸통  

  lcd.Fill_Rectangle(x + 25*sized, y + 3*sized, x + 37*sized, y + 15*sized); // 머리

  if (isSlide) lcd.Fill_Rectangle(x + 34*sized, y + 15*sized, x + 37*sized, y + 18*sized); // 코
  else lcd.Fill_Rectangle(x + 34*sized, y + 15*sized, x + 37*sized, y + 27*sized); // 코
  
  lcd.Set_Draw_color(WHITE); 
  lcd.Fill_Rectangle(x + 34*sized, y + 6*sized, x + 35*sized, y + 10*sized); // 눈
  if (isSlide) return;
  lcd.Set_Draw_color(BACKGROUND); // 다리 삭제
  if (sized == 1) lcd.Fill_Rectangle(x-2, y + 11 + 14, x + 27, y + 11 + 14 + 5);

  if(isJump)
  {
    lcd.Set_Draw_color(colors);
    lcd.Fill_Rectangle(x, y + 25*sized, x + 5*sized, y + 30*sized); // 뒷다리
    lcd.Fill_Rectangle(x + 20*sized, y + 25*sized, x + 25*sized, y + 30*sized); // 앞다리
    return;
  }
  
  lcd.Set_Draw_color(colors);
  if (currentMillis%320 < 80) {
    // 뒷다리
    lcd.Fill_Rectangle(x, y + 25, x + 5, y + 30);
    // 앞다리
    lcd.Fill_Rectangle(x + 20, y + 25, x + 25, y + 30);
  } 
  else if (currentMillis%320 >= 80 && currentMillis%320 < 160) {
    // 뒷다리
    lcd.Fill_Rectangle(x + 2, y + 11 + 14, x + 7, y + 11 + 14 + 5);
    // 앞다리
    lcd.Fill_Rectangle(x + 18, y + 11 + 14, x + 23, y + 11 + 14 + 5);
  } 
  else if (currentMillis%320 >= 160 && currentMillis%320 < 240) {
    // 뒷다리
    lcd.Fill_Rectangle(x + 4, y + 11 + 14, x + 9, y + 11 + 14 + 5);
    // 앞다리
    lcd.Fill_Rectangle(x + 16, y + 11 + 14, x + 21, y + 11 + 14 + 5);
  } 
  else if (currentMillis%320 >= 240) {
    // 뒷다리
    lcd.Fill_Rectangle(x + 2, y + 11 + 14, x + 7, y + 11 + 14 + 5);
    // 앞다리
    lcd.Fill_Rectangle(x + 18, y + 11 + 14, x + 23, y + 11 + 14 + 5);
  }
}

// 캐릭터 삭제 함수
void updateUserDisplay1(int16_t x,int16_t y) {
  lcd.Set_Draw_color(BACKGROUND); 

  // 몸통(껍질)
  lcd.Fill_Rectangle(x, y, x + 25, y + 11);
  lcd.Draw_Rectangle(x, y, x + 25, y + 11);

  // 몸통
  lcd.Fill_Rectangle(x, y + 11, x + 25, y + 11 + 14);
  // 머리
  lcd.Fill_Rectangle(x + 25, y + 3, x + 25 + 12, y + 3 + 12);
  // 눈
  lcd.Fill_Rectangle(x + 25 + 9, y + 6, x + 25 + 10, y + 6 + 4);
  if (types == 1) lcd.Fill_Rectangle(x + 29, y, x + 31, y + 3); // 귀
}

// 캐릭터 업데이트 함수 (모션시 업데이트 되는 부분 최소화)
void updateUserDisplay2(int16_t x, int16_t y, bool isJump, bool isSlide) {
  lcd.Set_Draw_color(BACKGROUND);
  // 몸통 아랫부분  업데이트
  lcd.Fill_Rectangle(x, y, x + 37, y + 3);
  
  // 머리 아래 부분 업데이트
  // 코끼리일 경우만 코 부분 업데이트
  if (types == 2) {
    if (isSlide) lcd.Fill_Rectangle(x + 34, y + 18, x + 37, y + 19); // 코
    else lcd.Fill_Rectangle(x + 34, y + 25, x + 37, y + 31); // 코
    lcd.Fill_Rectangle(x + 25, y + 14, x + 34, y + 17);
  }
  else lcd.Fill_Rectangle(x + 25, y + 14, x + 25 + 12, y + 17);

  // 다리부분 업데이트
  if (isJump) lcd.Fill_Rectangle(x, y + 30, x + 25, y + 34);
  
  
}

/*
// 캐릭터 전체 지우기
void allDelUser(int x,int y) {
  // 색상 지정
  lcd.Set_Draw_color(BACKGROUND); 
  // 껍질
  lcd.Fill_Rectangle(x, y, x + 25, y + 11);
  // 몸통
  lcd.Fill_Rectangle(x, y + 11, x + 25, y + 11 + 14);
  // 머리
  lcd.Fill_Rectangle(x + 25, y + 3, x + 25 + 12, y + 3 + 12);
  // 눈
  lcd.Fill_Rectangle(x + 25 + 9, y + 6, x + 25 + 10, y + 6 + 4);
  // 코
  lcd.Fill_Rectangle(x + 25+9, y + 3+12, x + 25 + 12, y + 27);
  // 귀
  lcd.Fill_Rectangle(x + 25 + 4, y, x + 25 + 6, y + 3); 
  //다리
  lcd.Fill_Rectangle(x, y + 25, x + 25, y + 30);  
}
// 아래 부분 캐릭터 지우기
void downDelUser(int x, int y) {
  lcd.Set_Draw_color(BACKGROUND);
  lcd.Fill_Rectangle(x, y + 25, x + 37, y + 32); // 다리
  lcd.Fill_Rectangle(x + 25, y + 13, x + 25 + 12, y + 16); // 머리
}

// 위 부분 캐릭터 지우기
void upDelUser(int x, int y) {
  lcd.Set_Draw_color(BACKGROUND);
  lcd.Fill_Rectangle(x, y-2, x + 37, y + 2); // 몸통
  lcd.Fill_Rectangle(x + 25, y + 13, x + 25 + 12, y + 17); // 머리
}

*/