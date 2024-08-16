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
int currentSlot = 0;
int currentGuess = 0;
int correct = 0;
int wrongSpot = 0;
int currentHint = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_NeoPixel pixels(28, slotLEDS, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel picking(4, selectionLEDS, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel answerLED(4, answerLEDS, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(leftButton, INPUT);
  pinMode(middleButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(selectionLEDS, OUTPUT);
  pinMode(slotLEDS, OUTPUT);
  pinMode(answerButton, INPUT);
  pinMode(upButton, INPUT);
  pinMode(downButton, INPUT);
  
  lcd.init();
  lcd.backlight();
  pixels.begin();
  answerLED.begin();
  randomSeed(analogRead(0));
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

    if(digitalRead(middleButton) == 1) {
      difficulty = 2;
      selectDifficulty = false;
      lcd.clear();
      for(int i = 0; i < 4; i++) {
        selectionLED[i] = 0;
        answer[i] = random(6);
        if(i == 1) {
          while(answer[i] == answer[i-1]) {
            answer[i] = random(6);
          }
        }
        if(i == 2) {
          while(answer[i] == answer[i-2] || answer[i] == answer[i-1]) {
            answer[i] = random(6);
          }
        }
        if(i == 3) {
          while(answer[i] == answer[i-3] || answer[i] == answer[i-2] || answer[i] == answer[i-1]) {
            answer[i] = random(6);
          }
        }
      }
      updateSelection();
      resetGuesses();
    }

    if(digitalRead(rightButton) == 1) {
      difficulty = 3;
      selectDifficulty = false;
      lcd.clear();
      for(int i = 0; i < 4; i++) {
        selectionLED[i] = 0;
        answer[i] = random(8);
        if(i == 1) {
          while(answer[i] == answer[i-1]) {
            answer[i] = random(8);
          }
        }
        if(i == 2) {
          while(answer[i] == answer[i-2] || answer[i] == answer[i-1]) {
            answer[i] = random(8);
          }
        }
        if(i == 3) {
          while(answer[i] == answer[i-3] || answer[i] == answer[i-2] || answer[i] == answer[i-1]) {
            answer[i] = random(8);
          }
        }
      }
      updateSelection();
      resetGuesses();
    }
  }

  if(digitalRead(leftButton) == 1) {
    if(currentSlot > 0) {
      currentSlot--;
      lcd.clear();
      delay(200);
    }
  }

  if(digitalRead(rightButton) == 1) {
    if(currentSlot < 3) {
      currentSlot++;
      lcd.clear();
      delay(200);
    }
  }

  if(digitalRead(middleButton) == 1) {
    switch(difficulty) {
      case 1:
        if(selectionLED[currentSlot] < 4) {
          selectionLED[currentSlot]++;
          delay(200);
        }
        if(selectionLED[currentSlot] == 4) {
          selectionLED[currentSlot] = 0;
        }
        break;
      case 2:
        if(selectionLED[currentSlot] < 6) {
          selectionLED[currentSlot]++;
          delay(200);
        }
        if(selectionLED[currentSlot] == 6) {
          selectionLED[currentSlot] = 0;
        }
        break;
      case 3:
        if(selectionLED[currentSlot] < 8) {
          selectionLED[currentSlot]++;
          delay(200);
        }
        if(selectionLED[currentSlot] == 8) {
          selectionLED[currentSlot] = 0;
        }
        break;
    }
    updateSelection();
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

void updateSelection() {
  for(int i = 0; i < 4; i++) {
    switch(selectionLED[i]) {
      case 0:
        picking.setPixelColor(i, pixels.Color(90, 90, 90));
        break;
      case 1:
        picking.setPixelColor(i, pixels.Color(240, 0, 0));
        break;
      case 2:
        picking.setPixelColor(i, pixels.Color(0, 240, 0));
        break;
      case 3:
        picking.setPixelColor(i, pixels.Color(0, 0, 240));
        break;
      case 4:
        picking.setPixelColor(i, pixels.Color(0, 140, 140));
        break;
      case 5:
        picking.setPixelColor(i, pixels.Color(140, 0, 140));
        break;
      case 6:
        picking.setPixelColor(i, pixels.Color(140, 140, 0));
        break;
      case 7:
        picking.setPixelColor(i, pixels.Color(140, 45, 75));
        break;
    }

    for(int i = 0; i < 4; i++) {
      switch(answer[i]) {
        case 0:
          answerLED.setPixelColor(i, pixels.Color(90, 90, 90));
          break;
        case 1:
          answerLED.setPixelColor(i, pixels.Color(240, 0, 0));
          break;
        case 2:
          answerLED.setPixelColor(i, pixels.Color(0, 240, 0));
          break;
        case 3:
          answerLED.setPixelColor(i, pixels.Color(0, 0, 240));
          break;
        case 4:
          answerLED.setPixelColor(i, pixels.Color(0, 140, 140));
          break;
        case 5:
          answerLED.setPixelColor(i, pixels.Color(140, 0, 140));
          break;
        case 6:
          answerLED.setPixelColor(i, pixels.Color(140, 140, 0));
          break;
        case 7:
          answerLED.setPixelColor(i, pixels.Color(140, 45, 75));
          break;
      }
    }
  }
  picking.show();
  answerLED.show();
}

void dropGuess() {
  for(int i = 0; i < 4; i++) {
    slotLED[currentGuess][i] = selectionLED[i];
    if(selectionLED[i] == answer[i]) {
      correct++;
    }
  }
  guessHint[currentGuess][0] = correct;

  if(selectionLED[0] == answer[1] || selectionLED[0] == answer[2] || selectionLED[0] == answer[3]) {
    wrongSpot++;
  }
  if(selectionLED[1] == answer[0] || selectionLED[1] == answer[2] || selectionLED[1] == answer[3]) {
    wrongSpot++;
  }
  if(selectionLED[2] == answer[1] || selectionLED[2] == answer[0] || selectionLED[2] == answer[3]) {
    wrongSpot++;
  }
  if(selectionLED[3] == answer[1] || selectionLED[3] == answer[2] || selectionLED[3] == answer[0]) {
    wrongSpot++;
  }

  guessHint[currentGuess][1] = wrongSpot;

  correctSpots = 0;
  for(int i = 0; i < 4; i++) {
    if(selectionLED[i] == answer[i]) {
      correctSpots++;
    }
  }

  if(currentGuess != 6) {
    currentGuess++;
    currentSlot = 0;
  } else {
    loser = true;
  }

  correct = 0;
  wrongSpot = 0;

  updateSelection();
  updateHint();
}

void resetGuesses() {
  for(int i = 0; i < 7; i++) {
    for(int j = 0; j < 4; j++) {
      slotLED[i][j] = 0;
    }
    guessHint[i][0] = 0;
    guessHint[i][1] = 0;
  }
  currentSlot = 0;
  currentGuess = 0;
}

void updateHint() {
  lcd.clear();
  for(int i = 0; i < currentGuess; i++) {
    lcd.setCursor(0,i);
    lcd.print(guessHint[i][0]);
    lcd.setCursor(1,i);
    lcd.print("-");
    lcd.setCursor(2,i);
    lcd.print(guessHint[i][1]);
  }

  if(correctSpots == 4) {
    winner = true;
  }
}

void winScreen() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("You won!");
  lcd.setCursor(0,1);
  lcd.print("Congrats!");
  while(true);
}

void loseScreen() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Game Over");
  lcd.setCursor(0,1);
  lcd.print("Try again!");
  while(true);
}
