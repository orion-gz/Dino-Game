void startGame() {
start_game:
  // 게임 설정 (배경, 점수, 캐릭터, 장애물, 난이도 등)
  initGame();

  // 게임이 끝날때까지 무한 반복
  while(1) {
    game();
    if(gameStatus == 1) {
      lcd.Fill_Rect(0, 0, lcdWidth, lcdHeight, CYAN);
      showMainMenu();
      return;
    }
    else if (gameStatus == 2)
      goto start_game;
  }
}

/* 게임 로직 */
// 게임 초기 설정 함수
void initGame() {
  switch (gameDifficulty) {
    case 0: obsSpeed = 4; break;
    case 1: obsSpeed = 5; break;
    case 2: obsSpeed = 6; break;
  }
  
  // 배경
  drawBackground();

  // 점수
  score = 0;

  // 캐릭터
  isJumping = false;
  isSliding = false;
  jumpHeight = -5;
  x = 50;
  y = 150;

  // 장애물
  /*
  for (int8_t i = 0; i < 12; ++i)
    mapSet[i] = 3;
  */
  obsX1 = 320;
  obsX2 = 400;
  obsX3 = 480;
  obsSize = random(15, 40);
  position = random(0, 5);

  currentObs1 = 0;
  currentObs2 = 1;
  currentObs3 = 2;

  // 게임 상태
  gameStatus = 0;
}

// 메인 게임 구현
void game() {
  // 점수 업데이트
  updateScore();
  movingCustomObs();
  // 장애물 움직임
  //movingObstacle();
  // 캐릭터 점프 움직임
  movingUser();

  if (gameControl() == 2)
    pause();
}

// 점수 업데이트 함수
void updateScore() {
  unsigned long currentMillis = millis();

  // 1초마다 점수 업데이트
  if (currentMillis - gameStartTime > 1000) {
    gameStartTime = currentMillis;
    lcd.Set_Draw_color(BACKGROUND);
    lcd.Fill_Rectangle(90, 15, 135, 30);
    lcd.Set_Draw_color(APRICOT);
    lcd.Fill_Rectangle(160, 198, 205, 215);

    if (gameDifficulty == 0) score += 1;
    else if (gameDifficulty == 1) score += 2;
    else if (gameDifficulty == 2) score += 3;

    lcd.Set_Text_Mode(true);
    lcd.Set_Text_Size(2);

    lcd.Set_Text_colour(RED);
    showString("Score: ", 15, 15, 2, RED, true);
    lcd.Print_Number_Int(score, 90, 15, 5, '0', 10);

    showString("1st Score: ", 15, 30, 2, BLUE, true);
    if(score < rankingData[0].score) lcd.Print_Number_Int(rankingData[0].score, 140, 30, 5, '0', 10);
    else {
      lcd.Set_Text_colour(RED);
      lcd.Set_Draw_color(BACKGROUND);
      lcd.Fill_Rectangle(140, 28, 185, 45);
      lcd.Print_Number_Int(score, 140, 30, 5, '0', 10);
    }
    
    showString("Time: ", 100, 200, 2, BLACK, true);
    if (gameDifficulty == 0) {
      lcd.Print_Number_Int(score, 160, 200, 5, '0', 10);
      showString("[Easy]", 120, 218, 2, BLUE, true);
    }
    else if (gameDifficulty == 1) { 
      lcd.Print_Number_Int(score / 2, 160, 200, 5, '0', 10);
      showString("[Soso]", 120, 218, 2, BLACK, true);
    }
    else if (gameDifficulty == 2) {
      lcd.Print_Number_Int(score / 3, 160, 200, 5, '0', 10);
      showString("[Hard]", 120, 218, 2, RED, true);
    }
  }
}


// 캐릭터 점프 움직임
void movingUser() {
  unsigned long currentMillis = millis();
  int8_t gc = gameControl();
  // 왼쪽 화면 터치시 점프 상태 활성화
  if (gc == 0 && y >= 150 && !isSliding) {
    isJumping = true;
    updateUserDisplay1(50, 150);
    jumpHeight = -1;
  }

  // 점프 상태가 true 일때 100까지 올라갔다가 내려옴
  if (isJumping && y >= 85 && y <= 150) {
    if (y <= 95) jumpHeight = 2;
    y += jumpHeight;
    
    // 캐릭터를 그리고 지우는 과정으로 점프 구현
    choiceUser(x, y, isJumping, isSliding);
    updateUserDisplay2(x, y, isJumping, isSliding);
    
    // 다시 원래 위치로 돌아오면 점프 상태 초기화
    if (y >= 150) {
      updateUserDisplay2(x, y, isJumping, isSliding);
      isJumping = false;
      y = 150;
      x = 50;
    }
  } 

  // 오른쪽 화면 터치시 슬라이딩 상태 활성화
  // 누르는동안 유지됨
  if (gc == 1 && !isJumping) {
    isSliding = true;
    updateUserDisplay1(50, 150);
    slideHeight = 2;
    slideStartTime = currentMillis;
  }
  
  // 슬라이딩 상태가 true 일때 160까지 내려갔다가 올라옴
  // 슬라이딩 상태 해제에는 0.1초가 필요
  if (isSliding) {
    if (currentMillis - slideStartTime < 100) {
      if (y >= 150 && y <= 160) y += slideHeight;
      choiceUser(x, y, isJumping, isSliding);
      updateUserDisplay2(x, y, isJumping, isSliding);
    } 
    else {
      if (y >= 150) {
        y -= slideHeight;
        choiceUser(x, y, isJumping, isSliding);
        updateUserDisplay2(x, y, isJumping, isSliding);
      }
      else { 
        isSliding = false;
        x = 50;
        y = 150;
      }
    }
  }

  if (!isSliding && !isJumping) choiceUser(50, 150, isJumping, isSliding);
}

// 커스텀된 장애물 움직임 함수
void movingCustomObs() {
  unsigned long currentMillis = millis();
  // 장애물이 완전히 화면에서 벗어났을때 장애물 상태 초기화
  if (currentObs1 <= maxMapSize) {
    if (obsX1 <= -20) {
      currentObs1 += 3; 
      obsX1 = 320;
    }
  }
  else 
    currentObs1 = 0;

  if (currentObs2 <= maxMapSize) {
    if (obsX2 <= -20) {
      currentObs2 += 3; 
      obsX2 = 320;
    }
  }
  else 
    currentObs2 = 1;
    
  if (currentObs3 <= maxMapSize) {
    if (obsX3 <= -20) {
      currentObs3 += 3; 
      obsX3 = 320;
    }
  }
  else 
    currentObs3 = 2;

  // 20 밀리 세컨드마다 장애물을 업데이트
  if (currentMillis - previousMillis >= 20) {
    previousMillis = currentMillis;
    lcd.Set_Draw_color(BACKGROUND);
    drawObs(mapSet[currentObs1], obsX1);
    drawObs(mapSet[currentObs2], obsX2);
    drawObs(mapSet[currentObs3], obsX3);
    obsX1 -= obsSpeed;
    obsX2 -= obsSpeed;
    obsX3 -= obsSpeed;
  } 
  else {
    lcd.Set_Draw_color(RED);
    drawObs(mapSet[currentObs1], obsX1);
    drawObs(mapSet[currentObs2], obsX2);
    drawObs(mapSet[currentObs3], obsX3);
  }

  // 장애물과 캐릭터와의 충돌을 확인
  if (checkGameOver(obsX1, mapSet[currentObs1]) || checkGameOver(obsX2, mapSet[currentObs2]) || checkGameOver(obsX3, mapSet[currentObs3]))
    gameOver();
}

// 장애물 움직임 
// x 좌표로만 움직임
/*
void movingObstacle() {
  unsigned long currentMillis = millis();
  int16_t obsVtxX1 = obsX - 15;
  int16_t obsVtxX2 = obsX;
  int16_t obsVtxY1 = obsPos[position] - obsSize;
  int16_t obsVtxY2 = obsPos[position];

  // 장애물이 완전히 화면에서 벗어났을때 장애물 상태 초기화
  // 랜덤으로 새로운 장애물을 생성
  if (obsX <= -5) {
    position = random(0, 5);
    obsSize = random(15, 40);
    obsX = 320;
  }

  // 20 밀리 세컨드마다 장애물을 업데이트
  if (currentMillis - previousMillis >= 20) {
    previousMillis = currentMillis;
    lcd.Set_Draw_color(BACKGROUND);
    lcd.Draw_Rectangle(obsVtxX1, obsVtxY1, obsVtxX2, obsVtxY2);
    obsX -= obsSpeed;
  } 
  else {
    lcd.Set_Draw_color(RED);
    lcd.Draw_Rectangle(obsVtxX1, obsVtxY1, obsVtxX2, obsVtxY2);
  }

  // 장애물과 캐릭터와의 충돌을 확인
  if (checkGameOver(obsVtxX1, obsVtxY1, obsVtxX2, obsVtxY2))
    gameOver();
}
*/

// 장애물과 캐릭터와의 충돌을 확인
// 캐릭터의 히트박스를 고려하여 계산    
// 몸통 히트박스 x<= Ax <= x+25, y <=Ax <= y+30 , 머리 히트박스 x+25<= Ax <=x+37, y+3<=Ax<=y+15
bool checkGameOver(int16_t obsX, int8_t obsType) {
  if (obsType == 1)
  {
    if ((x <= obsX + 20) && (x + 25 >= obsX) && (y <= 180) && (y + 30 >= 150))
      return true;
    if ((x+25 <= obsX + 20) && (x + 37 >= obsX) && (y+3 <= 180) && (y + 15 >= 150))
      return true;
  }

  if (obsType == 2)
  {
    if ((x <= obsX + 20) && (x + 25 >= obsX) && (y <= 160) && (y + 30 >= 45))
      return true;
    if ((x+25 <= obsX + 20) && (x + 37 >= obsX) && (y+3 <= 160) && (y + 15 >= 45))
      return true;
  }

  if (obsType == 3)
  {
    if ((x <= obsX + 20) && (x + 25 >= obsX) && (y <= 110) && (y + 30 >= 90))
      return true;
    if ((x+25 <= obsX + 20) && (x + 37 >= obsX) && (y+3 <= 110) && (y + 15 >= 90))
      return true;
  }

  return false;
}
/* 게임 로직 */

/* 게임 GUI */
// 게임 배경을 그리는 함수
void drawBackground() {
  // 배경
  lcd.Fill_Rect(0, 0, lcdWidth, lcdHeight, BACKGROUND);
  
  // 땅
  lcd.Set_Draw_color(APRICOT);
  lcd.Fill_Rectangle(0, 185, 320, 240);

   // 풀 (초록색으로 땅 위에 추가)
  lcd.Set_Draw_color(GREEN); 
  lcd.Fill_Rectangle(0, 185, 320, 195); 

  // 해
  lcd.Set_Draw_color(RED);
  lcd.Fill_Circle(320, 0, 45);
  lcd.Set_Draw_color(YELLOW);
  lcd.Fill_Circle(320, 0, 40);

  // 구름
  lcd.Set_Draw_color(WHITE);
  lcd.Fill_Rectangle(50, 25, 105, 40);
  lcd.Fill_Rectangle(40, 20, 90, 30);

  lcd.Fill_Rectangle(180, 35, 220, 45);
  lcd.Fill_Rectangle(185, 25, 215, 35);
  
  // pause 버튼 (해에 있는 눈)
  lcd.Set_Draw_color(BLACK);
  lcd.Fill_Rectangle(296, 10, 300, 22);
  lcd.Fill_Rectangle(310, 13, 314, 25);
  
}

// 장애물 그리는 함수
void drawObs(int8_t obs, int16_t nowObsX)
{
  switch(obs)
  {
    case 0: 
      break;
    case 1:
      lcd.Draw_Rectangle(nowObsX, 185, nowObsX + 20, 150 );
      lcd.Draw_Line(nowObsX, 185, nowObsX + 20, 185);
      break;
    case 2:
      lcd.Draw_Rectangle(nowObsX, 45, nowObsX + 20, 160 );
      lcd.Draw_Line(nowObsX, 160, nowObsX + 20, 160);
      break;
    case 3:
      lcd.Draw_Rectangle(nowObsX, 110, nowObsX + 20, 90);
      lcd.Draw_Line(nowObsX, 110, nowObsX + 20, 110);
      break;
  }
}

// 게임오버 메뉴
void gameOver()
{
  lcd.Set_Text_Size(4);
  lcd.Set_Text_colour(RED);
  lcd.Print("Game Over!", 55, 70);

  //lcd.Set_Draw_color(WHITE);
  //lcd.Draw_Rectangle(85, 105, 237, 125);
  //lcd.Fill_Rectangle(85, 105, 237, 125);

  lcd.Set_Text_Size(2);
  lcd.Set_Text_colour(BLUE);
  
  char scoreStr[10];
  sprintf(scoreStr, "Score : %d", score);
  lcd.Print(scoreStr, 105, 110);

  lcd.Set_Text_Size(2);
  lcd.Set_Draw_color(BLACK);
  lcd.Draw_Rectangle(16, 144, 104, 170);
  lcd.Fill_Rectangle(16, 144, 104, 170);
  showString("Restart", 21, 150, 2, WHITE, true);

  lcd.Draw_Rectangle(120, 144, 208, 170);
  lcd.Fill_Rectangle(120, 144, 208, 170);
  showString("Ranking", 124, 150, 2, WHITE, true);

  lcd.Draw_Rectangle(225, 144, 309, 170);
  lcd.Fill_Rectangle(225, 144, 309, 170);
  showString("Menu", 243, 150, 2, WHITE, true);
  
  while(1)
  {
    int8_t res = isGameMenuTouchDetected();

    if (res == 1) //restart (1리턴받음)
    {
      gameStatus = 2;
      break;
    }
    else if (res == 2) //ranking (2리턴받음)
    {
      rankingAdd();
      gameStatus = 1;
      break;
    }
    else if (res == 3) //menu (3리턴받음)
    {
      gameStatus = 1;
      break;
    }
  }
}

// pause 메뉴
void pause()
{
  lcd.Set_Text_Size(4);
  lcd.Set_Text_colour(RED);
  lcd.Print("Paused", 90, 70);

  lcd.Set_Text_Size(2);
  lcd.Set_Text_colour(BLUE);
  
  char scoreStr[10];
  sprintf(scoreStr, "Score : %d", score);
  lcd.Print(scoreStr, 105, 110);

  lcd.Set_Text_Size(2);
  lcd.Set_Draw_color(BLACK);
  lcd.Draw_Rectangle(16, 144, 104, 170);
  lcd.Fill_Rectangle(16, 144, 104, 170);
  showString("Resume", 25, 150, 2, WHITE, true);

  lcd.Draw_Rectangle(120, 144, 208, 170);
  lcd.Fill_Rectangle(120, 144, 208, 170);
  showString("Restart", 124, 150, 2, WHITE, true);

  lcd.Draw_Rectangle(225, 144, 309, 170);
  lcd.Fill_Rectangle(225, 144, 309, 170);
  showString("Menu", 243, 150, 2, WHITE, true);
  
  while(1)
  {
    int8_t res = isGameMenuTouchDetected();

    if (res == 1) { //resume (1리턴받음)
      drawBackground();
      break;
    }
    else if (res == 2) //restart (2리턴받음)
    {
      gameStatus = 2;
      break;
    }
    else if (res == 3) //menu (3리턴받음)
    {
      gameStatus = 1;
      break;
    }
  }
}

// 랭킹 추가 메뉴
void rankingAdd()
{
  lcd.Fill_Rect(0, 0, lcdWidth, lcdHeight, WHITE);
  lcd.Set_Text_Size(2); 

  lcd.Set_Draw_color(BLACK);
  lcd.Set_Text_colour(WHITE);
  lcd.Fill_Rectangle(18, 15, 150, 35);

  lcd.Print("Ranking Add", 20, 20);

  lcd.Set_Text_colour(BLACK);
  lcd.Print("Name : ", 20, 50);

  char scoreStr[10];
  sprintf(scoreStr, "Score : %d", score);
  lcd.Print(scoreStr, 20, 70);

  if(gameDifficulty == 0) {
    lcd.Print("Level : Easy", 20, 90);
  }
  else if(gameDifficulty == 1) {
    lcd.Print("Level : Soso", 20, 90);
  }
  else {
    lcd.Print("Level : Hard", 20, 90);
  }

  lcd.Draw_Line(15, 110, 300, 110);

  lcd.Set_Draw_color(RED);
  lcd.Fill_Rectangle(250, 190, 305, 215);
  showString("Set", 260, 195, 2, BLACK, true);

  drawKeyboard();

  String name = "";

  while(1)
  {
    char key = getKeyPressed(); // 터치 위치에 따라 눌린 키를 반환
    if (key != 0) { // 유효한 키 입력이 있는 경우에만 처리
      if (key == '*') { // DEL 키가 눌렸을 경우
        if (name.length() > 0) {
          name.remove(name.length() - 1); // 마지막 문자 제거
        }
      } 
      else {
        if(name.length() < 4) {
          name += key;
        }
      }
      
      lcd.Set_Text_colour(BLACK);
      uint8_t* clr = "          ";
      lcd.Print("Name :           ", 20, 50);
      lcd.Set_Draw_color(WHITE);
      lcd.Fill_Rectangle(100, 50, 320, 66);
      lcd.Print_String(name, 100, 50);
    }

    if(isSetTouchDetected())
    {
      String paddedName = name;
      while (paddedName.length() < 4) {
          paddedName = " " + paddedName;
      }
      updateRanking(paddedName, score, gameDifficulty);
      break;
    }
  }
}

// 랭킹 키보드 display
void drawKeyboard() {
    int keyX = 10, keyY = 130, keyW = 25, keyH = 25;
    
    lcd.Set_Draw_color(BLACK);
    // 산경식 코딩
    for (int i = 0; i < 26; i++) {
        lcd.Fill_Rectangle(keyX, keyY, keyX + keyW, keyY + keyH);
        
        if (i == 0) { showString("Q", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 1) { showString("W", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 2) { showString("E", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 3) { showString("R", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 4) { showString("T", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 5) { showString("Y", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 6) { showString("U", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 7) { showString("I", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 8) { showString("O", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 9) { showString("P", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 10) { showString("A", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 11) { showString("S", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 12) { showString("D", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 13) { showString("F", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 14) { showString("G", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 15) { showString("H", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 16) { showString("J", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 17) { showString("K", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 18) { showString("L", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 19) { showString("Z", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 20) { showString("X", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 21) { showString("C", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 22) { showString("V", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 23) { showString("B", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 24) { showString("N", keyX + 5, keyY + 5, 2, WHITE, true); }
        else if (i == 25) { showString("M", keyX + 5, keyY + 5, 2, WHITE, true); }
        keyX += keyW + 5;
        if ((i + 1) % 10 == 0) {
            keyY += keyH + 5;
            keyX = 10;
        }
      }
    // DEL 키
    lcd.Fill_Rectangle(keyX, keyY, keyX + keyW + 30, keyY + keyH);
    showString("DEL", keyX + 11, keyY + 5, 2, WHITE, true);
}
/* 게임 GUI */

/* Game Touch */
// 게임 터치 액션
int8_t gameControl() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > 10) {
    p.x = map(p.x, TS_MAXX, TS_MINX, 0, lcd.Get_Display_Height());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, lcd.Get_Display_Width());
    // pause 클릭
    if (p.y > 295 && p.y < 320)
      if (p.x > 0 && p.x < 30)
        return 2;

    // 왼쪽 클릭 (점프)
    if (p.y < 160) return 0; 
    // 오른쪽 클릭 (슬라이드)
    if (p.y > 160) return 1;
  }
  return -1;
}

// 게임 메뉴 터치 액션
// pause, gameover
int8_t isGameMenuTouchDetected() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
    p.x = map(p.x, TS_MAXX, TS_MINX, 0, lcd.Get_Display_Height());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, lcd.Get_Display_Width());

    // resume / restart 버튼 누름
    if (p.y >= 16 && p.y <= 104) 
      if (p.x >= 144 && p.x <= 170) return 1;

    // restart / ranking 버튼 누름
    if (p.y >= 120 && p.y <= 208) 
      if (p.x >= 144 && p.x <= 170) return 2;

    // menu 버튼 누름
    if (p.y >= 255 && p.y <= 309) 
      if (p.x >= 144 && p.x <= 170) return 3;
  }
  return 0;
}
/* Game Touch */