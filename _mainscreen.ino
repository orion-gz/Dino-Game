// 난이도 설정 함수
void difficultyCheck(int16_t x, int16_t y)
{
  // x 좌표 터치 범위
  if (x >= 170 && x <= 230)
  {
    // y 좌표 터치 범위
    // 오른쪽으로 난이도 조절 (>>)
    if (y >= 230 && y <= 290)
      gameDifficulty = (++gameDifficulty >= 3) ? 0 : gameDifficulty;
    // 왼쪽으로 난이도 조절 (<<)
    else if (y >= 130 && y <= 190)
      gameDifficulty = (--gameDifficulty < 0) ? 2 : gameDifficulty;
  }
  showMainMenu();
  //showDifficultyString(150, 200, 2, BLUE, true, gameDifficulty);
}

// 문자열 출력 함수
void showString(uint8_t *str,int16_t x,int16_t y,uint8_t csize, uint16_t fc,bool mode) {
  lcd.Set_Text_Mode(mode);
  lcd.Set_Text_Size(csize);
  lcd.Set_Text_colour(fc);
  //lcd.Set_Text_Back_colour(0XF800);
  lcd.Print_String(str, x, y);
}

/* 메인 GUI */
// 메인 메뉴 display 함수
// 메인 메뉴 display 함수
void showMainMenu() {
  lcd.Set_Draw_color(BLACK);
  lcd.Fill_Round_Rectangle(14, 14, 104, 61, 7);
  lcd.Fill_Round_Rectangle(14, 71, 104, 119, 7);
  lcd.Fill_Round_Rectangle(14, 129, 104, 176, 7);
  lcd.Fill_Round_Rectangle(14, 186, 104, 234, 7);
  
  lcd.Set_Draw_color(RED);
  // Start Button
  lcd.Fill_Round_Rectangle(10, 10, 100, 57, 7);
  showString("Start", 25, 25, 2, BLACK, true);

  // User Button
  //lcd.Draw_Rectangle(10, 67, 100, 115);
  lcd.Fill_Round_Rectangle(10, 67, 100, 115, 7);
  showString("User", 30, 82, 2, BLACK, true);

  // Map Button
  //lcd.Draw_Rectangle(10, 125, 100, 172);
  lcd.Fill_Round_Rectangle(10, 125, 100, 172, 7);
  showString("Map", 35, 140, 2, BLACK, true);

  // Rank Button
  //lcd.Draw_Rectangle(10, 182, 100, 230);  
  lcd.Fill_Round_Rectangle(10, 182, 100, 230, 7);
  showString("Rank", 30, 197, 2, BLACK, true);

  // Game Difficulty
  showDifficultyString(150, 200, 2, BLUE, true, gameDifficulty);

   // [CUSTOM GAME] 추가
  lcd.Set_Text_Size(3);
  if (gameDifficulty == 0) lcd.Set_Text_colour(BLUE); 
  else if (gameDifficulty == 1) lcd.Set_Text_colour(BLACK); 
  else if (gameDifficulty == 2) lcd.Set_Text_colour(RED); 
    
  lcd.Print(" CUSTOM GAME ", lcdWidth - 230, 20);
  if (gameDifficulty == 0) lcd.Set_Draw_color(BLUE);  
  else if (gameDifficulty == 1) lcd.Set_Draw_color(BLACK); 
  else if (gameDifficulty == 2) lcd.Set_Draw_color(RED);  

  lcd.Draw_Line(lcdWidth - 210, 50, lcdWidth - 15, 50);
  lcd.Draw_Line(lcdWidth - 210, 190, lcdWidth - 15, 190);
  nowChoiceUser(180, 120, 0);
  
  lcd.Set_Draw_color(GOLD);
  lcd.Fill_Circle(134, 66, 9);
  showString("1", 130, 60, 2, WHITE, true);
  lcd.Set_Draw_color(BRONZE);
  lcd.Fill_Circle(134, 86, 9);
  showString("2", 130, 80, 2, WHITE, true);
  lcd.Set_Draw_color(SILVER);
  lcd.Fill_Circle(134, 106, 9);
  showString("3", 130, 100, 2, WHITE, true);

  lcd.Set_Text_Size(2);

  mainRanking();
}

void mainRanking() {

  for(int i = 0; i < 3; ++i) {
    String formattedNickName = rankingData[i].playerName;
    String formattedScore = String(rankingData[i].score);

  int scoreLength = formattedScore.length();
  for (int j = 0; j < 4 - scoreLength; ++j) {
    formattedScore = "0" + formattedScore;
  }
  
  if(gameDifficulty == 0) lcd.Set_Text_colour(BLUE);
  else if(gameDifficulty == 1) lcd.Set_Text_colour(BLACK);
  else if(gameDifficulty == 2) lcd.Set_Text_colour(RED);

  String line = "             " + formattedNickName + " <" + formattedScore + ">";
  int yPosition = 60 + i * 20;
  lcd.Print(line.c_str(), 10, yPosition);   
  }
}

// 난이도 display 함수
void showDifficultyString(int16_t x, int16_t y, uint8_t csize, uint16_t fc, bool mode, int gameDifficulty) {
  uint8_t *str = "<< Easy >>";
  uint8_t* clr = "          ";

  lcd.Set_Text_Mode(mode);
  lcd.Set_Text_Size(csize);
  //lcd.Set_Text_Back_colour(0XF800);
  
  if (gameDifficulty == 1)
  {
    str = "<< Soso >>";
    fc = BLACK;
  }
  else if (gameDifficulty == 2)
  {
    str = "<< Hard >>";
    fc = RED;
  }

  lcd.Set_Text_colour(fc);
  lcd.Set_Draw_color(BACKGROUND);
  lcd.Fill_Rectangle(x, y, x + (strlen((char*)clr) * csize * 6), y + csize * 8);

  //글씨가 있던 영역을 배경색으로채워야함
  lcd.Print_String(str, x, y);

  delay(50);
}

/* 메인 GUI */

/* MainMenu Touch */
// 메인 화면 터치 액션
void mainMenuAction() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    p.x = map(p.x, TS_MAXX, TS_MINX, 0, lcd.Get_Display_Height());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, lcd.Get_Display_Width());
    difficultyCheck(p.x, p.y);
    
    if (p.y >= 10 && p.y <= 100) {
      // Start Button Click Event
      if (p.x >= 10 && p.x <= 57) startGame();
      // User Button Click Event
      if (p.x >= 67 && p.x <= 115) startUser();
      // Map Button Click Event
      if (p.x >= 125 && p.x <= 172) startMap();
      // Rank Button Click Event
      if (p.x >= 182 && p.x <= 230) startRanking();
    }   
  }
}
/* MainMenu Touch */