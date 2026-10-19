void startRanking() {
  lcd.Fill_Rect(0, 0, lcdWidth, lcdHeight, CYAN);  
  bool isUpdated = true; // 화면 갱신 여부를 확인하는 변수

  while(1)
  {
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    if(isRankingEndTouchDetected())
    {
      lcd.Fill_Rect(0, 0, lcdWidth, lcdHeight, CYAN);
      scrollIndex = 0; 
      showMainMenu();
      break;
    }

    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
      p.x = map(p.x, TS_MAXX, TS_MINX, 0, lcd.Get_Display_Height());
      p.y = map(p.y, TS_MINY, TS_MAXY, 0, lcd.Get_Display_Width());

      // 상단 터치 시 랭킹 위로 스크롤
      if (p.x < lcdHeight / 2 && scrollIndex > 0) {
        scrollIndex--;
        isUpdated = true;
      }
      // 하단 터치 시 랭킹 아래로 스크롤
      else if (p.x > lcdHeight / 2 && scrollIndex < maxRankingSize - displayRankingCount) {
        scrollIndex++;
        isUpdated = true;
      }
    }

    // 화면 갱신이 필요한 경우에만 ranking 함수 호출
    if (isUpdated) {
      ranking();
      isUpdated = false; // 화면 갱신 후 변수 초기화
    }
  }
}

void saveRankingToEEPROM() {
  int address = 0;
  for (int i = 0; i < maxRankingSize; i++) {
    EEPROM.put(address, rankingData[i]);
    address += sizeof(RankingData);
  }
}

// 랭킹 데이터 읽기
void loadRankingFromEEPROM() {
  int address = 0;
  for (int i = 0; i < maxRankingSize; i++) {
    EEPROM.get(address, rankingData[i]);
    address += sizeof(RankingData);
  }
}

// 랭킹 데이터 초기화 함수
void initRanking() {
  for (int i = 0; i < maxRankingSize; ++i) {
    strcpy(rankingData[i].playerName, "null"); // strcpy를 사용하여 문자열 복사
    rankingData[i].score = 0;
    rankingData[i].level = 0;
  }
  saveRankingToEEPROM();
}

void updateRanking(String playerName, int16_t score, int8_t level) {
  for (int8_t i = 0; i < maxRankingSize; ++i) {
    if (score > rankingData[i].score) {
      // 랭킹 갱신
      for (int j = maxRankingSize - 1; j > i; --j) {
        rankingData[j] = rankingData[j - 1];
      }
      char name[5];
      playerName.toCharArray(name, sizeof(name));
      strncpy(rankingData[i].playerName, name, sizeof(rankingData[i].playerName) - 1); // strncpy 사용
      rankingData[i].playerName[4] = '\0'; // 널 문자를 확실히 추가
      rankingData[i].score = score;
      rankingData[i].level = level;
      saveRankingToEEPROM(); // 갱신된 랭킹 데이터를 EEPROM에 저장
      break;
    }
  } 
}

/* Ranking GUI */
void ranking() {
  // 화면 지우기
  lcd.Fill_Rect(0, 0, lcdWidth, lcdHeight, CYAN);

  // 헤더 출력
  lcd.Set_Text_Size(2);
  lcd.Set_Text_colour(MAGENTA);
  lcd.Print("RANK", lcdWidth - 300, 20);
  lcd.Print("NAME", lcdWidth - 240, 20);
  lcd.Print("SCORE", lcdWidth - 171, 20);
  lcd.Print("LEVEL", lcdWidth - 85, 20);

  // 선 그리기
  lcd.Set_Draw_color(BLACK);
  lcd.Draw_Line(10, 40, 300, 40);

  // scrollIndex부터 랭킹 데이터 표시
  for (int i = 0; i < displayRankingCount; ++i) {
    int index = scrollIndex + i;
    if (index >= maxRankingSize) break; // 더 이상 출력할 데이터가 없으면 중단

    lcd.Set_Text_Size(2);
    lcd.Set_Text_colour(BLACK);

    // 닉네임 및 점수 형식화
    String formattedNickName = rankingData[index].playerName;
    String formattedScore = String(rankingData[index].score);
    int scoreLength = formattedScore.length();
    for (int j = 0; j < 4 - scoreLength; ++j) {
      formattedScore = "0" + formattedScore;
    }

    // 레벨 문자열
    String levelString;
    switch (rankingData[index].level) {
      case 0: levelString = "Easy"; break;
      case 1: levelString = "Soso"; break;
      case 2: levelString = "Hard"; break;
      default: levelString = "Unknown";
    }

    // 랭킹 색상 변경
    if (index == 0) {
      lcd.Set_Text_colour(RED);
    } else if (index == 1) {
      lcd.Set_Text_colour(APRICOT);
    } else if (index == 2) {
      lcd.Set_Text_colour(BLUE);
    } else {
      lcd.Set_Text_colour(BLACK);
    }

    // 랭킹 출력
    String line = " [" + String(index + 1) + "]  " + formattedNickName + " <" + formattedScore + ">  " + levelString;
    int yPosition = 50 + i * 20;
    lcd.Print(line.c_str(), 10, yPosition);
  }

    lcd.Set_Draw_color(RED);
    lcd.Set_Text_colour(BLACK);
    lcd.Fill_Rectangle(300, 13, 317, 30);
    showString("B", 304, 15, 2, BLACK, true);
}
/* Ranking GUI */

/* */

/* */

/* RankingMenu Touch */
// 랭킹 터치 액션 Set(저장)
bool isSetTouchDetected() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
    p.x = map(p.x, TS_MAXX, TS_MINX, 0, lcd.Get_Display_Height());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, lcd.Get_Display_Width());

    if (p.y >= 250 && p.y <= 305) 
    {
      if (p.x >= 190 && p.x <= 215) 
      {
        return true;
      }
    }
    return false;
  }
  else return false;
}

// 랭킹 터치 액션 back
bool isRankingEndTouchDetected() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) 
  {
    p.x = map(p.x, TS_MAXX, TS_MINX, 0, lcd.Get_Display_Height());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, lcd.Get_Display_Width());

    if (p.y >= 300 && p.y <= 317) {
      if (p.x >= 13 && p.x <= 30) {
        return true;
      }
    }
  }
  else return false;
}

// 랭킹 키보드 터치액션
// 터치시 해당 문자 반환
char getKeyPressed() {
    TSPoint p = ts.getPoint();

    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
        // 터치 스크린 좌표를 실제 화면 좌표로 변환
        p.x = map(p.x, TS_MAXX, TS_MINX, 0, lcd.Get_Display_Height());
        p.y = map(p.y, TS_MINY, TS_MAXY, 0, lcd.Get_Display_Width());

        int keyX = 10, keyY = 130, keyW = 25, keyH = 25;
        char keys[] = "QWERTYUIOPASDFGHJKLZXCVBNM*"; // 'DEL' 키 포함

        for(int i = 0; i < 27; ++i)
        {
          if (i == 26)
          {
            keyW += 30;
          }
          if (p.y >= keyX && p.y <= keyX + keyW)
          {
            if (p.x >= keyY && p.x <= keyY + keyH)
            {
              delay(200);
              if(i == 26)
              {
                return keys[26];
              }
              return keys[i];
            }
          }

          keyX += keyW + 5;
          if ((i + 1) % 10 == 0) {
            keyY += keyH + 5;
            keyX = 10;
          }
        }
    }
    return 0; // 키가 눌리지 않았을 경우
}
/* RankingMenu Touch */