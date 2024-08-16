#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>

#define leftButton 4
#define middleButton 5
#define rightButton 6
#define selectionLEDS 9


int slotLED[7][4];
int selectionLED[4];
int answer[4];
int guessHint[7][2];

bool selectDifficulty = true;
bool winner = false;
bool loser = false;
int correctSpots = 0;
int difficulty;


LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_NeoPixel pixels(28, slotLEDS, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel picking(4, selectionLEDS, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(leftButton, INPUT);
  pinMode(middleButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(selectionLEDS, OUTPUT);
  pinMode(slotLEDS, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  pixels.begin();
  answerLED.begin();
}

void loop() {
  while(selectDifficulty) {
    lcd.setCursor(0,0);
    lcd.print("Difficulty:");
    lcd.setCursor(0,1);
    lcd.print("Easy  Med   Hard");

    if(digitalRead(leftButton) == 1) {
      difficulty = 1;
      selectDifficulty = false;
      lcd.clear();
      for(int i = 0; i < 4; i++) {
        selectionLED[i] = 0;
        answer[i] = random(4);
        if(i == 1) {
          while(answer[i] == answer[i-1]) {
            answer[i] = random(4);
          }
        }
        if(i == 2) {
          while(answer[i] == answer[i-2] || answer[i] == answer[i-1]) {
            answer[i] = random(4);
          }
        }
        if(i == 3) {
          while(answer[i] == answer[i-3] || answer[i] == answer[i-2] || answer[i] == answer[i-1]) {
            answer[i] = random(4);
          }
        }
      }
      updateSelection();
      resetGuesses();
    }

    

  if(selectDifficulty != true) {
    switch(currentSlot) {
      case 0:
        lcd.setCursor(0,1);
        lcd.print("()");
        break;
      case 1:
        lcd.setCursor(4,1);
        lcd.print("()");
        break;
      case 2:
        lcd.setCursor(9,1);
        lcd.print("()");
        break;
      case 3:
        lcd.setCursor(14,1);
        lcd.print("()");
        break;
    }
  }

  if(digitalRead(answerButton) == 1) {
    dropGuess();
  }

  updateHint();

  if(winner) {
    winScreen();
  }

  if(loser) {
    loseScreen();
  }
}
