#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>

#define leftButton 5
#define middleButton 6
#define rightButton 7
#define selectionLEDS 10
#define slotLEDS 8
#define answerLEDS 9
#define answerButton 4
#define upButton 3
#define downButton 2

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

LiquidCrystal_I2C lcd(0x20, 16, 2);
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
  while (selectDifficulty) {
    lcd.setCursor(0, 0);
    lcd.print("Difficulty:");
    lcd.setCursor(0, 1);
    lcd.print("Easy  Med   Hard");

    if (digitalRead(leftButton) == 1) {
      difficulty = 1;
      selectDifficulty = false;
      lcd.clear();
      for (int i = 0; i < 4; i++) {
        selectionLED[i] = 0;
        answer[i] = random(3);
        if (i == 1) {
          while (answer[i] == answer[i - 1]) {
            answer[i] = random(4);
          }
        }
        if (i == 2) {
          while (answer[i] == answer[i - 2] || answer[i] == answer[i - 1]) {
            answer[i] = random(4);
          }
        }
        if (i == 3) {
          while (answer[i] == answer[i - 3] || answer[i] == answer[i - 2] || answer[i] == answer[i - 1]) {
            answer[i] = random(4);
          }
        }
      }
      updateSelection();
      resetGuesses();
    }

    if (digitalRead(middleButton) == 1) {
      difficulty = 2;
      selectDifficulty = false;
      lcd.clear();
      for (int i = 0; i < 4; i++) {
        selectionLED[i] = 0;
        answer[i] = random(6);
        if (i == 1) {
          while (answer[i] == answer[i - 1]) {
            answer[i] = random(6);
          }
        }
        if (i == 2) {
          while (answer[i] == answer[i - 2] || answer[i] == answer[i - 1]) {
            answer[i] = random(6);
          }
        }
        if (i == 3) {
          while (answer[i] == answer[i - 3] || answer[i] == answer[i - 2] || answer[i] == answer[i - 1]) {
            answer[i] = random(6);
          }
        }
      }
      updateSelection();
      resetGuesses();
    }

    if (digitalRead(rightButton) == 1) {
      difficulty = 3;
      selectDifficulty = false;
      lcd.clear();
      for (int i = 0; i < 4; i++) {
        selectionLED[i] = 0;
        answer[i] = random(8);
        if (i == 1) {
          while (answer[i] == answer[i - 1]) {
            answer[i] = random(8);
          }
        }
        if (i == 2) {
          while (answer[i] == answer[i - 2] || answer[i] == answer[i - 1]) {
            answer[i] = random(8);
          }
        }
        if (i == 3) {
          while (answer[i] == answer[i - 3] || answer[i] == answer[i - 2] || answer[i] == answer[i - 1]) {
            answer[i] = random(8);
          }
        }
      }
      updateSelection();
      resetGuesses();
    }
  }

  if (digitalRead(leftButton) == 1) {
    if (currentSlot > 0) {
      currentSlot--;
      lcd.clear();
      delay(200);
    }
  }

  if (digitalRead(rightButton) == 1) {
    if (currentSlot < 4) {
      currentSlot++;
      lcd.clear();
      delay(200);
    }
  }

  if (digitalRead(middleButton) == 1) {
    switch (difficulty) {
      case 1:
        if (selectionLED[currentSlot] < 4) {
          selectionLED[currentSlot]++;
          delay(200);
        }
        if (selectionLED[currentSlot] == 4) {
          selectionLED[currentSlot] = 0;
        }
        break;
      case 2:
        if (selectionLED[currentSlot] < 6) {
          selectionLED[currentSlot]++;
          delay(200);
        }
        if (selectionLED[currentSlot] == 6) {
          selectionLED[currentSlot] = 0;
        }
        break;
      case 3:
        if (selectionLED[currentSlot] < 8) {
          selectionLED[currentSlot]++;
          delay(200);
        }
        if (selectionLED[currentSlot] == 8) {
          selectionLED[currentSlot] = 0;
        }
        break;
    }
    updateSelection();
  }

  if (selectDifficulty != true) {
    switch (currentSlot) {
      case 0:
        lcd.setCursor(0, 1);
        lcd.print("()");
        break;
      case 1:
        lcd.setCursor(4, 1);
        lcd.print("()");
        break;
      case 2:
        lcd.setCursor(9, 1);
        lcd.print("()");
        break;
      case 3:
        lcd.setCursor(14, 1);
        lcd.print("()");
        break;
    }
  }

  if (digitalRead(answerButton) == 1) {
    dropGuess();
  }

  updateHint();

  if (winner) {
    winScreen();
  }

  if (loser) {
    loseScreen();
  }
}

void updateSelection() {
  for (int i = 0; i < 4; i++) {
    switch (selectionLED[i]) {
      case 0:
        picking.setPixelColor(i, pixels.Color(100, 100, 100));
        break;
      case 1:
        picking.setPixelColor(i, pixels.Color(255, 0, 0));
        break;
      case 2:
        picking.setPixelColor(i, pixels.Color(0, 255, 0));
        break;
      case 3:
        picking.setPixelColor(i, pixels.Color(0, 0, 255));
        break;
      case 4:
        picking.setPixelColor(i, pixels.Color(0, 155, 155));
        break;
      case 5:
        picking.setPixelColor(i, pixels.Color(155, 0, 155));
        break;
      case 6:
        picking.setPixelColor(i, pixels.Color(155, 155, 0));
        break;
      case 7:
        picking.setPixelColor(i, pixels.Color(155, 50, 80));
        break;
    }

    for (int i = 0; i < 4; i++) {
      switch (answer[i]) {
        case 0:
          answerLED.setPixelColor(i, pixels.Color(100, 100, 100));
          break;
        case 1:
          answerLED.setPixelColor(i, pixels.Color(255, 0, 0));
          break;
        case 2:
          answerLED.setPixelColor(i, pixels.Color(0, 255, 0));
          break;
        case 3:
          answerLED.setPixelColor(i, pixels.Color(0, 0, 255));
          break;
        case 4:
          answerLED.setPixelColor(i, pixels.Color(0, 155, 155));
          break;
        case 5:
          answerLED.setPixelColor(i, pixels.Color(155, 0, 155));
          break;
        case 6:
          answerLED.setPixelColor(i, pixels.Color(155, 155, 0));
          break;
        case 7:
          answerLED.setPixelColor(i, pixels.Color(155, 50, 80));
          break;
      }
    }
  }
  picking.show();
  answerLED.show();
}

void dropGuess() {
  for (int i = 0; i < 4; i++) {
    slotLED[currentGuess][i] = selectionLED[i];
    if (selectionLED[i] == answer[i]) {
      correct++;
    }
    for (int j = 0; j < 4; j++) {
      if (selectionLED[i] == answer[j] && i != j) {
        wrongSpot++;
      }
    }
  }
  guessHint[currentHint][0] = correct;
  guessHint[currentHint][1] = wrongSpot;
  currentGuess++;
  correct = 0;
  wrongSpot = 0;
  currentHint++;
  currentSlot = 0;
  if (guessHint[6][0] == 4) {
    winner = true;
  }
  if (currentGuess > 6 && guessHint[6][0] != 4) {
    loser = true;
  }
}

void updateHint() {
  for (int i = 0; i < currentHint; i++) {
    lcd.setCursor(0, i);
    lcd.print(guessHint[i][0]);
    lcd.setCursor(2, i);
    lcd.print("Right");
    lcd.setCursor(8, i);
    lcd.print(guessHint[i][1]);
    lcd.setCursor(10, i);
    lcd.print("Off");
  }
}

void winScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Congrats!");
  lcd.setCursor(0, 1);
  lcd.print("You won!!");
  while (true) {
  }
}

void loseScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Oops...");
  lcd.setCursor(0, 1);
  lcd.print("You lost!");
  while (true) {
  }
}

void resetGuesses() {
  for (int i = 0; i < 7; i++) {
    guessHint[i][0] = 0;
    guessHint[i][1] = 0;
    for (int j = 0; j < 4; j++) {
      slotLED[i][j] = 0;
    }
  }
  currentHint = 0;
  currentGuess = 0;
}
