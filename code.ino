#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>

#define leftButton 4
#define middleButton 5
#define rightButton 6
#define selectionLEDS 9
#define slotLEDS 7
#define answerLEDS 8
#define answerButton 3
#define upButton 2
#define downButton 1
#define hintButton 10

int slotLED[7][4];
int selectionLED[4];
int answer[4];
int guessHint[7][2];
int score = 0; 
unsigned long guessTimeLimit = 10000;
unsigned long guessStartTime = 0;

bool selectDifficulty = true;
bool winner = false;
bool loser = false;
bool hintUsed = false;
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
  pinMode(hintButton, INPUT);  
  
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
      generateAnswer(4); 
      updateSelection();
      resetGuesses();
    }

    if(digitalRead(middleButton) == 1) {
      difficulty = 2;
      selectDifficulty = false;
      lcd.clear();
      generateAnswer(6);
      updateSelection();
      resetGuesses();
    }

    if(digitalRead(rightButton) == 1) {
      difficulty = 3;
      selectDifficulty = false;
      lcd.clear();
      generateAnswer(8);
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
    cycleColor(); 
  }

  if(selectDifficulty != true) {
    displayCursor();
  }

  if(digitalRead(answerButton) == 1 || millis() - guessStartTime > guessTimeLimit) {
    dropGuess();
  }

  if(digitalRead(hintButton) == 1 && !hintUsed) {
    giveHint();
  }

  updateHint();

  if(winner) {
    winScreen();
  }

  if(loser) {
    loseScreen();
  }
}

void generateAnswer(int colorRange) {
  for(int i = 0; i < 4; i++) {
    selectionLED[i] = 0;
    answer[i] = random(colorRange);
    for(int j = 0; j < i; j++) {
      while(answer[i] == answer[j]) {
        answer[i] = random(colorRange);
      }
    }
  }
}

void cycleColor() {
  switch(difficulty) {
    case 1:
      selectionLED[currentSlot] = (selectionLED[currentSlot] + 1) % 4;
      break;
    case 2:
      selectionLED[currentSlot] = (selectionLED[currentSlot] + 1) % 6;
      break;
    case 3:
      selectionLED[currentSlot] = (selectionLED[currentSlot] + 1) % 8;
      break;
  }
  updateSelection();
}

void displayCursor() {
  lcd.clear();
  lcd.setCursor(currentSlot * 5, 1);
  lcd.print("()");
}

void dropGuess() {
  guessStartTime = millis();  
  for(int i = 0; i < 4; i++) {
    slotLED[currentGuess][i] = selectionLED[i];
    if(selectionLED[i] == answer[i]) {
      correct++;
      score += 10;  
    }
  }
  guessHint[currentGuess][0] = correct;

  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      if(i != j && selectionLED[i] == answer[j]) {
        wrongSpot++;
        score += 5;  
      }
    }
  }
  guessHint[currentGuess][1] = wrongSpot;

  if(correct == 4) {
    winner = true;
  } else if(currentGuess == 6) {
    loser = true;
  } else {
    currentGuess++;
    currentSlot = 0;
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
  guessStartTime = millis();
  score = 0;
}

void updateHint() {
  lcd.clear();
  for(int i = 0; i < currentGuess; i++) {
    lcd.setCursor(0, i);
    lcd.print(guessHint[i][0]);
    lcd.setCursor(1, i);
    lcd.print("-");
    lcd.setCursor(2, i);
    lcd.print(guessHint[i][1]);
  }

  lcd.setCursor(12, 0);
  lcd.print("Score:");
  lcd.setCursor(12, 1);
  lcd.print(score);
}

void giveHint() {
  hintUsed = true;
  for(int i = 0; i < 4; i++) {
    if(selectionLED[i] != answer[i]) {
      selectionLED[i] = answer[i];
      score -= 20; 
      updateSelection();
      break;
    }
  }
}

void winScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("You won!");
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(score);
  while(true);
}

void loseScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game Over");
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(score);
  while(true);
}
