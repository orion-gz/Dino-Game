#include "_define.h"

void setup() {
  //initRanking(); // 랭킹 초기화
  loadRankingFromEEPROM(); // EEPROM에서 랭킹 데이터 로드
  initDisplay();
}

void loop() {
  mainMenuAction();
}

void initDisplay() {
  lcd.Init_LCD();
  lcd.Set_Rotation(1);
  lcd.Fill_Screen(BACKGROUND);
  showMainMenu();
  pinMode(13, OUTPUT);
}