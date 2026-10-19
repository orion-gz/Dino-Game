#include <LCDWIKI_GUI.h>
#include <LCDWIKI_KBV.h>
#include <TouchScreen.h> //touch library
#include <EEPROM.h>

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8 

//touch sensitivity for X
#define TS_MINX 124
#define TS_MAXX 906

//touch sensitivity for Y
#define TS_MINY 83
#define TS_MAXY 893

// We have a status line for like, is FONA working
#define STATUS_X 10
#define STATUS_Y 65

//touch sensitivity for press
#define MINPRESSURE 10
#define MAXPRESSURE 1000

// 기본 색
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define APRICOT 0xFEA0
#define GRAY 0x1082
#define BACKGROUND CYAN

// 메달 색
#define GOLD 0xFFD700
#define SILVER 0xC0C0C0 
#define BRONZE 0xCD7F32



LCDWIKI_KBV lcd(ILI9341,A3,A2,A1,A0,A4);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

int16_t lcdWidth = 320;      // TFT LCD의 너비
int16_t lcdHeight = 240;   

/* 게임 전역 변수 */
// 게임 상태
// 0 : 게임 진행중, 1 : 게임 종료, 2 : 게임 재시작
int8_t gameStatus = 0;

// 난이도
int8_t gameDifficulty = 0;

// 점수
unsigned long gameStartTime = 0;
int16_t score;

// 캐릭터 
unsigned long slideStartTime = 0;
bool isJumping;
bool isSliding;
int8_t jumpHeight;
int8_t slideHeight;
int16_t x;
int16_t y;

// 캐릭터 선택
int8_t types = 0; // 캐릭터 타입 선택 0:거북이 1:젖소 2:코끼리
int16_t colors = GREEN;

// 장애물
int8_t currentObs1 = 0;
int8_t currentObs2 = 1;
int8_t currentObs3 = 2;
unsigned long previousMillis = 0;
int16_t obsX1;
int16_t obsX2;
int16_t obsX3;
int8_t obsSize;
int16_t obsPos[5] = { 100, 130, 185, 185, 185 };
int8_t obsSpeed;
int8_t position;

// 맵 설정
int8_t touchObs = 0; // 어떤 장애물을 터치했는가(맵 설정에서 사용)
int8_t nowMap = 0; // 지금 맵 위치(맵 설정에서 사용)
int8_t mapSet[100] = {0}; // 맵 설정 0:없음 1:하단 2:상단 3:중단 장애물
int8_t maxMapSize = 98;
/* 게임 전역 변수 */

/* 게임 랭킹 전역 변수 */
struct RankingData {    
  char playerName[5];
  int16_t score;
  int8_t level;
};

const int8_t maxRankingSize = 20; // 랭킹 최대 사이즈
const int8_t displayRankingCount = 9;  // 예시로 10개의 항목을 동시에 표시한다고 가정
RankingData rankingData[maxRankingSize];
/* 게임 랭킹 전역 변수 */

int scrollIndex = 0; // 랭킹 화면에서 현재 표시되는 랭킹 목록의 시작 인덱스